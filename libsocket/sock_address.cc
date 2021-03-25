#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include "sock_address.h"
#include "sock_log.h"

namespace infra::sock {

SockAddress::SockAddress()
{
    this->init_flag_ = false;
    this->multicast_flag_ = false;
}

SockAddress::SockAddress(const SockAddress& other)
{
    init_flag_ = other.init_flag_;
    family_ = other.family_;
    multicast_flag_ = other.multicast_flag_;
    address_ = other.address_;
    port_ = other.port_;
    domain_ = other.domain_;
    type_ = other.type_;
}

SockAddress::SockAddress(const SockAddress&& other)
{
    init_flag_ = other.init_flag_;
    family_ = other.family_;
    multicast_flag_ = other.multicast_flag_;
    address_ = other.address_;
    port_ = other.port_;
    domain_ = other.domain_;
    type_ = other.type_;
}

const SockAddress& SockAddress::operator=(const SockAddress& other)
{
    return SockAddress::operator=(std::move(other));
}
const SockAddress& SockAddress::operator=(const SockAddress&& other)
{
    init_flag_ = other.init_flag_;
    family_ = other.family_;
    multicast_flag_ = other.multicast_flag_;
    address_ = other.address_;
    port_ = other.port_;
    domain_ = other.domain_;
    type_ = other.type_;
    return *this;
}

std::string SockAddress::getAddr()
{
    return this->address_;
}

SockRet SockAddress::_init(SockFamily family, unsigned short int port)
{
    return _init(family, NULL, port);
}

SockRet SockAddress::_init(SockFamily family, const char* address, unsigned short int port)
{
    switch(family){
        case SockFamily::TCP_INET4:
            this->domain_ = AF_INET;
            this->type_ = SOCK_STREAM;
            break;
        case SockFamily::TCP_INET6:
            this->domain_ = AF_INET6;
            this->type_ = SOCK_STREAM;
            break;
        case SockFamily::UDP_INET4:
            this->domain_ = AF_INET;
            this->type_ = SOCK_DGRAM;
            break;
        case SockFamily::UDP_INET6:
            this->domain_ = AF_INET6;
            this->type_ = SOCK_DGRAM;
            break;
        case SockFamily::MULTICAST_INET4:
            this->domain_ = AF_INET;
            this->type_ = SOCK_DGRAM;
            this->multicast_flag_ = true;
            break;
        case SockFamily::MULTICAST_INET6:
            this->domain_ = AF_INET6;
            this->type_ = SOCK_DGRAM;
            this->multicast_flag_ = true;
            break;
        case SockFamily::TCP_LOCAL:
        case SockFamily::UDP_LOCAL:
            SOCK_ERROR("%s", "Local socket need unixfile not port!");
            this->init_flag_ = false;
            return SockRet::EINIT;
        default:
            SOCK_ERROR("%s", "Unknow socket family!");
            this->init_flag_ = false;
            return SockRet::EINIT;
    }
    this->family_ = family;
    this->port_ = port;    
    this->address_ = address ? address : "";
    this->init_flag_ = true;
    return SockRet::SUCCESS;
}

SockRet SockAddress::_init(SockFamily family, const char* address)
{
    switch(family){
        case SockFamily::TCP_LOCAL:
            this->domain_ = AF_LOCAL;
            this->type_ = SOCK_STREAM;
            break;
        case SockFamily::UDP_LOCAL:
            this->domain_ = AF_LOCAL;
            this->type_ = SOCK_DGRAM;
            break;
        case SockFamily::TCP_INET4:
        case SockFamily::TCP_INET6:
        case SockFamily::UDP_INET4:
        case SockFamily::UDP_INET6:
        case SockFamily::MULTICAST_INET4:
        case SockFamily::MULTICAST_INET6:
            SOCK_ERROR("%s", "Inet socket need port!");
            this->init_flag_ = false;
            return SockRet::EINIT;
        default:
            SOCK_ERROR("%s", "Unknow socket family!");
            this->init_flag_ = false;
            return SockRet::EINIT;
    }
    this->family_ = family;
    this->address_ = address;
    this->init_flag_ = true;
    return SockRet::SUCCESS;
}

SockRet SockAddress::ToSockaddr(struct sockaddr* addr)
{
    if (!this->init_flag_) {
        SOCK_ERROR("%s", "Not initialized");
        return SockRet::EINIT;
    }
    if (!addr) {
        SOCK_ERROR("%s", "Bad arguments");
        return SockRet::EBADARGS;
    }

    switch(this->domain_){
        case AF_LOCAL:
            ((struct sockaddr_un*)addr)->sun_family = this->domain_;
            strcpy(((struct sockaddr_un*)addr)->sun_path, this->address_.c_str());
            break;
        case AF_INET:
            ((struct sockaddr_in*)addr)->sin_family = this->domain_;
            if (this->address_.empty()) {
                ((struct sockaddr_in*)addr)->sin_addr.s_addr = htonl(INADDR_ANY);
            } else {
                ((struct sockaddr_in*)addr)->sin_addr.s_addr = htonl(inet_addr(this->address_.c_str()));
            }   
            ((struct sockaddr_in*)addr)->sin_port = htons(this->port_);
            break;
        case AF_INET6:
            ((struct sockaddr_in6*)addr)->sin6_family = AF_INET6;
            if (this->address_.empty()) {
                ((struct sockaddr_in6*)addr)->sin6_addr = in6addr_any;
            } else {
                if (inet_pton(AF_INET6, this->address_.c_str(), &(((struct sockaddr_in6*)addr)->sin6_addr)) < 0) {
                    SOCK_ERROR("Address is not in presentation format[%s]", this->address_.c_str());
                    return SockRet::ERROR;
                }
            }
            ((struct sockaddr_in6*)addr)->sin6_port = htons(this->port_);
            break;
        default:
            SOCK_ERROR("%s", "Unknow socket family!");
            return SockRet::ERROR;
    }
    return SockRet::SUCCESS;
}

bool SockAddress::AddrCheck()
{
    return this->init_flag_;
}

bool SockAddress::isMulticast()
{
    return this->multicast_flag_;
}

}//namespace infra::sock end
