#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include "file_path.h"

#include "sock_log.h"
#include "sock_server.h"

#define SOCK_DEFAULT_LISTEN_CACHE (100)

namespace infra::sock {

SockServer::SockServer()
{
    init_flag_ = false;
}

SockServer::SockServer(SockFamily family, unsigned short int port)
{
    s_address_ = SockAddress(family, port);
    if (!s_address_.AddrCheck()) {
        init_flag_ = false;
        return;
    }
    listen_cache_ = SOCK_DEFAULT_LISTEN_CACHE;
    init_flag_ = true;
    auto_close_flag_ = false;
}

SockServer::SockServer(SockFamily family, const char* address, unsigned short int port)
{
    s_address_ = SockAddress(family, address, port);
    if (!s_address_.AddrCheck()) {
        init_flag_ = false;
        return;
    }
    listen_cache_ = SOCK_DEFAULT_LISTEN_CACHE;
    init_flag_ = true;
    auto_close_flag_ = false;
}

SockServer::SockServer(SockFamily family, const char* address)
{
    s_address_ = SockAddress(family, address);
    if (!s_address_.AddrCheck()) {
        init_flag_ = false;
        return;
    }
    listen_cache_ = SOCK_DEFAULT_LISTEN_CACHE;
    init_flag_ = true;
    auto_close_flag_ = false;
}

SockServer::SockServer(SockAddress* address)
{
    s_address_ = SockAddress(*address);
    if (!s_address_.AddrCheck()) {
        init_flag_ = false;
        return;
    }
    listen_cache_ = SOCK_DEFAULT_LISTEN_CACHE;
    init_flag_ = true;
    auto_close_flag_ = false;
}

SockServer::SockServer(const SockServer& other)
{
    init_flag_ = other.init_flag_;
    auto_close_flag_ = other.auto_close_flag_;
    s_address_ = other.s_address_;
    listen_cache_ = other.listen_cache_;
    listen_fd_ = other.listen_fd_;
}

SockServer::~SockServer()
{
}

const SockServer& SockServer::operator=(const SockServer& other)
{
    init_flag_ = other.init_flag_;
    auto_close_flag_ = other.auto_close_flag_;
    s_address_ = other.s_address_;
    listen_cache_ = other.listen_cache_;
    listen_fd_ = other.listen_fd_;
    return *this;
}

SockFD& SockServer::GetSockFD()
{
    return listen_fd_;
}

SockRet SockServer::Bind()
{
    if (s_address_.domain_ == AF_LOCAL) {
    }
    SockRet ret;
    if (!init_flag_) {
        SOCK_ERROR("%s", "Not initialized");
        return SockRet::EINIT;
    }
    if ((ret = _socket()) != SockRet::SUCCESS) {
        return ret;
    }
    if ((ret = _bind()) != SockRet::SUCCESS) {
        listen_fd_.Close();
        return ret;
    }
    if (s_address_.type_ == SOCK_STREAM) {
        if ((ret = _listen()) != SockRet::SUCCESS) {
            listen_fd_.Close();
            return ret;
        }
    } else if (s_address_.type_ == SOCK_DGRAM) {
        //if multicast server and given multicast group, add socket to this multicast group
        if (s_address_.isMulticast() && !s_address_.address_.empty()) {
            if ((ret = listen_fd_.SetMcastJoin(s_address_.address_.c_str())) != SockRet::SUCCESS) {
                listen_fd_.Close();
                return ret;
            }
        }
    }
    return SockRet::SUCCESS;
}

SockRet SockServer::Accept(SockFD* sockfd)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "Not initialized");
        return SockRet::ERROR;
    }
    if (s_address_.type_ == SOCK_DGRAM) {
        SOCK_ERROR("%s", "UDP socket no need be accepted");
        return SockRet::ERROR;
    }
    int temp_errno;
    int acpt_fd = -1;
    switch(s_address_.domain_) {
        case AF_LOCAL:
            struct sockaddr_un un_addr;
            socklen_t un_addrlen;
            acpt_fd = accept(listen_fd_.GetFD(), (struct sockaddr*)&un_addr, &un_addrlen);
            if (acpt_fd < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s%s", "Accept socket error, ", strerror(temp_errno));
                return (temp_errno);
            }
            sockfd->SetFD(acpt_fd);
            sockfd->orig = s_address_;
            sockfd->dest = SockAddress(s_address_.family_, un_addr.sun_path);
            break;
        case AF_INET:
            struct sockaddr_in in_addr;
            socklen_t in_addrlen;
            acpt_fd = accept(listen_fd_.GetFD(), (struct sockaddr*)&in_addr, &in_addrlen);
            if (acpt_fd < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s%s", "Accept socket error, ", strerror(temp_errno));
                return (temp_errno);
            }
            sockfd->SetFD(acpt_fd);
            sockfd->orig = s_address_;
            sockfd->dest = SockAddress(s_address_.family_, inet_ntoa(in_addr.sin_addr), in_addr.sin_port);
            break;
        case AF_INET6:
            struct sockaddr_in6 in6_addr;
            char c_in6_addr[SOCK_ADDRESS_MAX_LEN];
            socklen_t in6_addrlen;
            acpt_fd = accept(listen_fd_.GetFD(), (struct sockaddr*)&in6_addr, &in6_addrlen);
            if (acpt_fd < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s%s", "Accept socket error, ", strerror(temp_errno));
                return (temp_errno);
            }
            if (!inet_ntop(AF_INET6, &in6_addr.sin6_addr, c_in6_addr, SOCK_ADDRESS_MAX_LEN)) {
                temp_errno = errno;
                SOCK_ERROR("%s%s", "Accept socket error, inet_ntop error", strerror(temp_errno));
                return (temp_errno);
            }
            sockfd->SetFD(acpt_fd);
            sockfd->orig = s_address_;
            sockfd->dest = SockAddress(s_address_.family_, c_in6_addr, in6_addr.sin6_port);
            break;
        default:
            SOCK_ERROR("Unkown destnation family[%d]", s_address_.type_);
            break;
    }
    return SockRet::SUCCESS;
}

SockRet SockServer::_socket()
{
    int temp_errno;
    int sockfd = 0;
    int on = 1;
    sockfd = socket(s_address_.domain_, s_address_.type_, 0);
    if (sockfd < 0) {
        temp_errno = errno;
        SOCK_ERROR("%s%s", "Create socket error, ", strerror(temp_errno));
        return (temp_errno);
    }
    if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        temp_errno = errno;
        SOCK_ERROR("%s%s", "Create socket error, ", strerror(temp_errno));
        return (temp_errno);
    }
    listen_fd_.SetFD(sockfd, auto_close_flag_);
    listen_fd_.orig = s_address_;
    return SockRet::SUCCESS;
}

SockRet SockServer::_bind()
{
    int ret;
    int temp_errno;
    if (s_address_.domain_ == AF_LOCAL) {
        struct sockaddr_un addr;
        addr.sun_family = AF_LOCAL;

        file::FilePath addr_path(s_address_.address_);
        if (addr_path.GetPath(addr_path.GetDepth()).size() > sizeof(addr.sun_path)) {
            return SockRet::SOCK_EADDRESS;
        }
        strcpy(addr.sun_path, addr_path.GetPath(addr_path.GetDepth()).c_str());
        
        if (chdir(addr_path.GetPath(0, addr_path.GetDepth() - 1).c_str()) < 0) {
            return errno;
        }
        unlink(addr.sun_path);
        ret = bind(listen_fd_.GetFD(), (struct sockaddr*)&addr, sizeof(struct sockaddr));
        if (ret != 0) {
            temp_errno = errno;
            SOCK_ERROR("%s%s", "bind socket error, ", strerror(temp_errno));
            unlink(s_address_.address_.c_str());
            return (temp_errno);
        }
    } else if (s_address_.domain_ == AF_INET) {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        if (s_address_.address_.empty() || s_address_.isMulticast()) {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else {
            addr.sin_addr.s_addr = htonl(inet_addr(s_address_.address_.c_str()));
        }
        addr.sin_port = htons(s_address_.port_);

        ret = bind(listen_fd_.GetFD(), (struct sockaddr*)&addr, sizeof(struct sockaddr));
        if (ret != 0) {
            temp_errno = errno;
            SOCK_ERROR("%s%s", "bind socket error, ", strerror(temp_errno));
            return (temp_errno);
        }
    } else if (s_address_.domain_ == AF_INET6 || s_address_.isMulticast()) {
        struct sockaddr_in6 addr;
        addr.sin6_family = AF_INET6;
        if (s_address_.address_.empty()) {
            addr.sin6_addr = in6addr_any;
        } else {
          ret = inet_pton(AF_INET6, s_address_.address_.c_str(), &(addr.sin6_addr));
          if (ret < 0) {
            temp_errno = errno;
            SOCK_ERROR("Address is not in presentation format[%s]", s_address_.address_.c_str());
            return (temp_errno);
          }
        }
        addr.sin6_port = htons(s_address_.port_);

        ret = bind(listen_fd_.GetFD(), (struct sockaddr*)&addr, sizeof(struct sockaddr_in6));
        if (ret != 0) {
            temp_errno = errno;
            SOCK_ERROR("%s%s", "bind socket error, ", strerror(temp_errno));
            return (temp_errno);
        }
    } else {
        SOCK_ERROR("%s", "Unknow domain for socket");
        return SockRet::ERROR;
    }
    return SockRet::SUCCESS;
}

SockRet SockServer::_listen()
{
    int temp_errno;
    int ret = listen(listen_fd_.GetFD(), listen_cache_);
    if (ret) {
        temp_errno = errno;
        SOCK_ERROR("%s%s", "Listen socket error, ", strerror(temp_errno));
        return (temp_errno);
    }
    return SockRet::SUCCESS;
}

} //namespace infra::sock end
