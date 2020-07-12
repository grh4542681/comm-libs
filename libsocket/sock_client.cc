#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include "file_path.h"

#include "sock_log.h"
#include "sock_client.h"

namespace sock{

SockClient::SockClient()
{
    init_flag_ = false;
}

SockClient::SockClient(SockFamily family, const char* address, unsigned short int port)
{
    s_address_ = SockAddress(family, address, port);
    if (!s_address_.AddrCheck()) {
        init_flag_ = false;
        return;
    }   
    init_flag_ = true;
    auto_close_flag_ = false;
}

SockClient::SockClient(SockFamily family, const char* address)
{
    s_address_ = SockAddress(family, address);
    if (!s_address_.AddrCheck()) {
        init_flag_ = false;
        return;
    }   
    init_flag_ = true;
    auto_close_flag_ = false;
}

SockClient::SockClient(SockAddress* address)
{
    s_address_ = SockAddress(*address);
    if (!s_address_.AddrCheck()) {
        init_flag_ = false;
        return;
    }   
    init_flag_ = true;
    auto_close_flag_ = false;
}

SockClient::SockClient(const SockClient& other)
{
    init_flag_ = other.init_flag_;
    auto_close_flag_ = other.auto_close_flag_;
    s_address_ = other.s_address_;
    conn_fd_ = other.conn_fd_;
}

SockClient::~SockClient()
{
}

const SockClient& SockClient::operator=(const SockClient& other)
{
    init_flag_ = other.init_flag_;
    auto_close_flag_ = other.auto_close_flag_;
    s_address_ = other.s_address_;
    conn_fd_ = other.conn_fd_;
    return *this;
}

SockFD& SockClient::GetSockFD()
{
    return conn_fd_;
}

SockRet SockClient::SetTimeout(struct timeval* overtime)
{
    SOCK_DEBUG("Not support now");
    return SockRet::SUCCESS;
}

void SockClient::SetAutoClose(bool flag)
{
    auto_close_flag_ = flag;
    conn_fd_.SetAutoClose(flag);
}

SockRet SockClient::Connect()
{
    SockRet ret;
    if (!init_flag_) {
        SOCK_ERROR("%s", "Not initialized")
        return SockRet::EINIT;
    }
    if ((ret = _socket()) != SockRet::SUCCESS) {
        return ret;
    }
    if (s_address_.type_ == SOCK_DGRAM) {
        SOCK_DEBUG("%s", "UDP no need connect");
    } else {
        if ((ret = _connect()) != SockRet::SUCCESS) {
            return ret;
        }
    }
    return SockRet::SUCCESS;
}

//private
SockRet SockClient::_socket()
{
    int temp_errno;
    int sockfd = 0;
    sockfd = socket(s_address_.domain_, s_address_.type_, 0);
    if (sockfd < 0) {
        temp_errno = errno;
        SOCK_ERROR("%s%s", "Create socket error, ", strerror(temp_errno));
        return (temp_errno);
    }
    conn_fd_.SetFD(sockfd, auto_close_flag_);
    conn_fd_.dest = s_address_;
    return SockRet::SUCCESS;
}

SockRet SockClient::_connect()
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

        ret = connect(conn_fd_.GetFD(), (struct sockaddr*)&addr, sizeof(struct sockaddr));
        if (ret != 0) {
            temp_errno = errno;
            SOCK_ERROR("%s%s", "connect socket error, ", strerror(temp_errno));
            unlink(s_address_.address_.c_str());
            return (temp_errno);
        }
    } else if (s_address_.domain_ == AF_INET) {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        if (s_address_.address_.empty()) {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else {
            addr.sin_addr.s_addr = htonl(inet_addr(s_address_.address_.c_str()));
        }
        addr.sin_port = htons(s_address_.port_);

        ret = connect(conn_fd_.GetFD(), (struct sockaddr*)&addr, sizeof(struct sockaddr));
        if (ret != 0) {
            temp_errno = errno;
            SOCK_ERROR("%s%s", "connect socket error, ", strerror(temp_errno));
            return (temp_errno);
        }
    } else if (s_address_.domain_ == AF_INET6) {
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

        ret = connect(conn_fd_.GetFD(), (struct sockaddr*)&addr, sizeof(struct sockaddr_in6));
        if (ret != 0) {
            temp_errno = errno;
            SOCK_ERROR("%s%s", "connect socket error, ", strerror(temp_errno));
            return (temp_errno);
        }
    } else {
        SOCK_ERROR("%s", "Unknow domain for socket");
        return SockRet::ERROR;
    }
    return SockRet::SUCCESS;

}

}
