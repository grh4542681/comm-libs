#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "sock_log.h"
#include "sock_fd.h"

namespace infra::sock{
//public
/**
* @brief SockFD - Default consturctor
*/
SockFD::SockFD() : FD(){
    fd_ = 0;
    init_flag_ = false;
}

/**
* @brief SockFD - Constructor
*
* @param [fd] - File descriptor
* @param [auto_close] - Automatically close when destructuring
*/
SockFD::SockFD(unsigned int fd, bool auto_close) : FD(fd, auto_close){
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        SOCK_ERROR("%s", strerror(temp_errno));
        init_flag_ = false;
    }
    if (!S_ISSOCK(fd_stat.st_mode)) {
        SOCK_ERROR("fd[%d] not a socket fd", fd);
        init_flag_ = false;
    }

    init_flag_ = true;
}

SockFD::SockFD(const SockFD& other) : FD(other) {
    orig = other.orig;
    dest = other.dest;
}

const SockFD& SockFD::operator=(const SockFD& other) {
    io::FD::operator=(other);
    orig = other.orig;
    dest = other.dest;
    return *this;
}

/**
* @brief ~SockFD - Destructor.
*/
SockFD::~SockFD(){
    if (init_flag_ && auto_close_) {
        _close();
    }
}

/**
* @brief SetFD - Set or reset file descriptors.
*
* @param [fd] - File descriptor
* @param [auto_close] - Automatically close when destructuring
*
* @returns  SockRet
*/
ret::Return SockFD::SetFD(unsigned int fd, bool auto_close)
{
    if (fd_ > 0 && init_flag_ && auto_close_) {
        _close();
        fd_ = 0;
    }
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        SOCK_ERROR("%s", strerror(temp_errno));
        init_flag_ = false;
    }
    if (!S_ISSOCK(fd_stat.st_mode)) {
        SOCK_ERROR("fd[%d] not a socket fd", fd);
        init_flag_ = false;
    }
    fd_ = fd;
    init_flag_ = true;
    auto_close_ = auto_close;
    return SockRet::SUCCESS;
}

ret::Return SockFD::Dup(io::FD& new_fd)
{
    ret::Return ret = SockRet::SUCCESS;
    ret = io::FD::Dup(new_fd);
    if (ret != SockRet::SUCCESS) {
        return ret;
    } else {
        dynamic_cast<SockFD&>(new_fd).SetOrigAddress(orig);
        dynamic_cast<SockFD&>(new_fd).SetDestAddress(dest);
        return SockRet::SUCCESS;
    }
}

io::FD* SockFD::Clone() {
    return alloc_.Allocate<SockFD>(*this);
}

SockAddress& SockFD::GetOrigAddress() {
    return orig;
}

SockAddress& SockFD::GetDestAddress() {
    return dest;
}

SockRet SockFD::SetOrigAddress(SockAddress& addr) {
    orig = addr;
    return SockRet::SUCCESS;
}

SockRet SockFD::SetDestAddress(SockAddress& addr) {
    dest = addr;
    return SockRet::SUCCESS;
}

/**
* @brief SetMcastJoin - Add the current socket descriptor to the multicast address.
*
* @param [mcast_addr] - Multicast address.
*
* @returns  SockRet.
*/
SockRet SockFD::SetMcastJoin(const char* mcast_addr)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    return SetMcastJoin(mcast_addr, NULL, 0);
}

/**
* @brief SetMcastJoin - Add the current socket descriptor to the multicast address
*                       If you specify both if_name and if_index,Ipv6 used if_index
*                       and Ipv4 used if_name.
*
* @param [mcast_addr] - Multicast address.
* @param [if_name] - Interface name.
* @param [if_index] - Interface index
*
* @returns  
*/
SockRet SockFD::SetMcastJoin(const char* mcast_addr, const char* if_name, unsigned int if_index)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    if (!isMulitcastFD()) {
        return SockRet::ENONSUPPORT;
    }
    int temp_errno = 0;
    struct ip_mreq mreq;
    struct ipv6_mreq mreq6;
    SockAddress* paddr = orig.AddrCheck() ? &(orig) : &(dest);
    switch (paddr->domain_) {
        case AF_INET:
            memset(&mreq, 0x00, sizeof(struct ip_mreq));
            mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr);
            if (if_name || if_index) {
                struct ifreq ifreq;
                memset(&ifreq, 0x00, sizeof(struct ifreq));
                if (if_name) {
                    memcpy(ifreq.ifr_name, if_name, IFNAMSIZ);
                } else {
                    if (if_indextoname(if_index, ifreq.ifr_name) == NULL) {
                        temp_errno = errno;
                        SOCK_ERROR("%s", strerror(temp_errno));
                        return (temp_errno);
                    }
                }
                if (ioctl(fd_, SIOCGIFADDR, &ifreq) < 0){
                    temp_errno = errno;
                    SOCK_ERROR("%s", strerror(temp_errno));
                    return (temp_errno);
                }
                memcpy(&mreq.imr_interface, &((struct sockaddr_in*)&ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
            } else {
                mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            }
            if (setsockopt(fd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        case AF_INET6:
            memset(&mreq6, 0x00, sizeof(struct ipv6_mreq));
            if (inet_pton(AF_INET6, mcast_addr, &mreq6.ipv6mr_multiaddr) < 0) {
                    SOCK_ERROR("Address is not in presentation format[%s]", mcast_addr);
                    return SockRet::ERROR;
            }
            if (if_name || if_index > 0) {
                if (if_name) {
                    if ((mreq6.ipv6mr_interface = if_nametoindex(if_name)) == 0) {
                        temp_errno = errno;
                        SOCK_ERROR("%s", strerror(temp_errno));
                        return (temp_errno);
                    }
                } else {
                    mreq6.ipv6mr_interface = if_index;
                }
            } else {
                mreq6.ipv6mr_interface = 0;
            }
            if (setsockopt(fd_, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq6, sizeof(struct ipv6_mreq)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        default:
            SOCK_ERROR("Unknow domain[%d]", paddr->domain_);
            return SockRet::ERROR;
    }
    return SockRet::SUCCESS;
}

/**
* @brief SetMcastLeave - Remove the current socket descriptor from the multicast address
*
* @param [mcast_addr] - Multicast address.
*
* @returns  SockRet.
*/
SockRet SockFD::SetMcastLeave(const char* mcast_addr)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    if (!isMulitcastFD()) {
        return SockRet::ENONSUPPORT;
    }
    int temp_errno = 0;
    struct ip_mreq mreq;
    struct ipv6_mreq mreq6;
    SockAddress* paddr = orig.AddrCheck() ? &(orig) : &(dest);
    switch (paddr->domain_) {
        case AF_INET:
            memset(&mreq, 0x00, sizeof(struct ip_mreq));
            mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr);
            mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            if (setsockopt(fd_, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        case AF_INET6:
            memset(&mreq6, 0x00, sizeof(struct ipv6_mreq));
            if (inet_pton(AF_INET6, mcast_addr, &mreq6.ipv6mr_multiaddr) < 0) {
                    SOCK_ERROR("Address is not in presentation format[%s]", mcast_addr);
                    return SockRet::ERROR;
            }
            mreq6.ipv6mr_interface = 0;
            if (setsockopt(fd_, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, &mreq6, sizeof(struct ipv6_mreq)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        default:
            SOCK_ERROR("Unknow domain[%d]", paddr->domain_);
            return SockRet::ERROR;
    }
    return SockRet::SUCCESS;
}

/**
* @brief SetMcastInterface - Modify current multicast interface.
*                            If you specify both if_name and if_index,Ipv6 used if_index
*                            and Ipv4 used if_name.
*
* @param [if_name] - Interface name.
* @param [if_index] - Interface index.
*
* @returns  SockRet.
*/
SockRet SockFD::SetMcastInterface(const char* if_name, unsigned int if_index)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    if (!isMulitcastFD()) {
        return SockRet::ENONSUPPORT;
    }
    int temp_errno = 0;
    struct in_addr addr;
    unsigned int index = 0;
    SockAddress* paddr = orig.AddrCheck() ? &(orig) : &(dest);
    switch (paddr->domain_) {
        case AF_INET:
            memset(&addr, 0x00, sizeof(struct in_addr));
            if (if_name || if_index > 0) {
                struct ifreq ifreq;
                memset(&ifreq, 0x00, sizeof(struct ifreq));
                if (if_name) {
                    memcpy(ifreq.ifr_name, if_name, IFNAMSIZ);
                } else {
                    if (if_indextoname(if_index, ifreq.ifr_name) == NULL) {
                        temp_errno = errno;
                        SOCK_ERROR("%s", strerror(temp_errno));
                        return (temp_errno);
                    }
                }
                if (ioctl(fd_, SIOCGIFADDR, &ifreq) < 0){
                    temp_errno = errno;
                    SOCK_ERROR("%s", strerror(temp_errno));
                    return (temp_errno);
                }
                memcpy(&addr, &((struct sockaddr_in*)&ifreq.ifr_addr)->sin_addr, sizeof(struct in_addr));
            } else {
                return SockRet::EBADARGS;
            }
            if (setsockopt(fd_, IPPROTO_IP, IP_MULTICAST_IF, &addr, sizeof(struct in_addr)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        case AF_INET6:
            if (if_name || if_index > 0) {
                if (if_name) {
                    if ((index = if_nametoindex(if_name)) == 0) {
                        temp_errno = errno;
                        SOCK_ERROR("%s", strerror(temp_errno));
                        return (temp_errno);
                    }
                } else {
                    index = if_index;
                }
            } else {
                return SockRet::EBADARGS;
            }
            if (setsockopt(fd_, IPPROTO_IPV6, IPV6_MULTICAST_IF, &index, sizeof(index)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        default:
            SOCK_ERROR("Unknow domain[%d]", paddr->domain_);
            return SockRet::ERROR;
    }
    return SockRet::SUCCESS;
}

/**
* @brief SetMcastTTL - Set the TTL value of the multicast group data
*
* @param [ttl] - TTL value.
*
* @returns  SockRet.
*/
SockRet SockFD::SetMcastTTL(unsigned int ttl)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    if (!isMulitcastFD()) {
        return SockRet::ENONSUPPORT;
    }
    int temp_errno = 0;
    SockAddress* paddr = orig.AddrCheck() ? &(orig) : &(dest);
    switch (paddr->domain_) {
        case AF_INET:
            if (setsockopt(fd_, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        case AF_INET6:
            if (setsockopt(fd_, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &ttl, sizeof(ttl)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        default:
            SOCK_ERROR("Unknow domain[%d]", paddr->domain_);
            return SockRet::ERROR;
    }
    return SockRet::SUCCESS;
}

/**
* @brief SetMcastloop - Disable multicast data echo
*
* @param [flag] - true/false.
*
* @returns  SockRet.
*/
SockRet SockFD::SetMcastloop(bool flag)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    if (!isMulitcastFD()) {
        return SockRet::ENONSUPPORT;
    }
    int temp_errno = 0;
    unsigned char loopflag = flag ? '1' : '\0';
    unsigned int loopflag6 = flag ? 1 : 0;
    SockAddress* paddr = orig.AddrCheck() ? &(orig) : &(dest);
    switch (paddr->domain_) {
        case AF_INET:
            if (setsockopt(fd_, IPPROTO_IP, IP_MULTICAST_LOOP, &loopflag, sizeof(loopflag)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        case AF_INET6:
            if (setsockopt(fd_, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loopflag6, sizeof(loopflag6)) < 0) {
                temp_errno = errno;
                SOCK_ERROR("%s", strerror(temp_errno));
                return (temp_errno);
            }
            break;
        default:
            SOCK_ERROR("Unknow domain[%d]", paddr->domain_);
            return SockRet::ERROR;
    }
    return SockRet::SUCCESS;
}

/**
* @brief SetSendBlock - Set Blocked when send msg
*
* @param [overtime] - Over time, if NULL, it will be blocked all time.
*
* @returns  SockRet
*/
SockRet SockFD::SetSendBlock(timer::Time* overtime)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    int temp_errno = 0;
    struct timeval otime;
    int flag = 0;
    flag = fcntl(fd_, F_GETFL);
    if (flag < 0) {
        temp_errno = errno;
        SOCK_ERROR("%s", strerror(temp_errno));
        return (temp_errno);
    }
    if (flag & O_NONBLOCK) {
        flag &= (~O_NONBLOCK);
        if (fcntl(fd_, F_SETFL, &flag) < 0) {
            temp_errno = errno;
            SOCK_ERROR("%s", strerror(temp_errno));
            return (temp_errno);
        }
    }
    if (overtime) {
        overtime->To(&otime);
    } else {
        otime.tv_sec = 0;
        otime.tv_usec = 0;
    }
    if (setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &otime, sizeof(struct timeval)) < 0) {
        temp_errno = errno;
        SOCK_ERROR("%s", strerror(temp_errno));
        return (temp_errno);
    }
    return SockRet::SUCCESS;
}

/**
* @brief SetRecvBlock - Set blocked when recv msg
*
* @param [overtime] - Over time, if NULL, it will be blocked all the time
*
* @returns  SockRet
*/
SockRet SockFD::SetRecvBlock(timer::Time* overtime)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    int temp_errno = 0;
    struct timeval otime;
    int flag = 0;
    flag = fcntl(fd_, F_GETFL);
    if (flag < 0) {
        temp_errno = errno;
        SOCK_ERROR("%s", strerror(temp_errno));
        return (temp_errno);
    }
    if (flag & O_NONBLOCK) {
        flag &= (~O_NONBLOCK);
        if (fcntl(fd_, F_SETFL, &flag) < 0) {
            temp_errno = errno;
            SOCK_ERROR("%s", strerror(temp_errno));
            return (temp_errno);
        }
    }
    if (overtime) {
        overtime->To(&otime);
    } else {
        otime.tv_sec = 0;
        otime.tv_usec = 0;
    }
    if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &otime, sizeof(struct timeval)) < 0) {
        temp_errno = errno;
        SOCK_ERROR("%s", strerror(temp_errno));
        return (temp_errno);
    }
    return SockRet::SUCCESS;
}

/**
* @brief SetNonBlock - Set recv and send msg non-block
*
* @returns  SockRet
*/
SockRet SockFD::SetNonBlock()
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return SockRet::EINIT;
    }
    int temp_errno = 0;
    int flag = 0;
    flag = fcntl(fd_, F_GETFL);
    if (flag < 0) {
        temp_errno = errno;
        SOCK_ERROR("%s", strerror(temp_errno));
        return (temp_errno);
    }
    if (!(flag & O_NONBLOCK)) {
        flag |= O_NONBLOCK;
        if (fcntl(fd_, F_SETFL, &flag) < 0) {
            temp_errno = errno;
            SOCK_ERROR("%s", strerror(temp_errno));
            return (temp_errno);
        }
    }
    return SockRet::SUCCESS;
}

/**
* @brief isClientFD - Check this FD was created by client.
*
* @returns  true or false
*/
bool SockFD::isClientFD()
{
    if (!orig.AddrCheck() && dest.AddrCheck()) {
        return true;
    }
    return false;
}

/**
* @brief isServerFD - Check this FD was created by server.
*
* @returns  true or false
*/
bool SockFD::isServerFD()
{
    if (orig.AddrCheck() && !dest.AddrCheck()) {
        return true;
    }
    return false;
}

/**
* @brief isAcceptFD - Check this FD was created by accept.
*
* @returns  true or false
*/
bool SockFD::isAcceptFD()
{
    if (orig.AddrCheck() && dest.AddrCheck()) {
        return true;
    }
    return false;
}

/**
* @brief isMulitcastFD - Check this FD was joined in multicast.
*
* @returns  true or false.
*/
bool SockFD::isMulitcastFD()
{
    SockAddress* paddr = orig.AddrCheck() ? &(orig) : &(dest);
    return paddr->multicast_flag_;
}

/**
* @brief Close - Close FD
*/
void SockFD::Close()
{
    if (init_flag_) {
        _close();
    }
    init_flag_ = false;
}

/**
* @brief Send - Send message
*
* @param [data] - Data pointer that will be sended.
* @param [datalen] - Length of data.
*
* @returns  SockRet or send length
*/
ssize_t SockFD::Write(const void* data, size_t datalen)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return (-1);
    }
    SockAddress* tmp = isClientFD() ? &(this->dest) : &(this->orig);
    if (tmp->type_ == SOCK_DGRAM) {
        SockAddress* dest = &(this->dest);
        if (!dest->AddrCheck()) {
            SOCK_ERROR("UDP send need destnation address");
            return (-1);
        }
        struct sockaddr dest_addr;
        memset(&dest_addr, 0x00, sizeof(struct sockaddr));
        if (dest->ToSockaddr(&dest_addr) != SockRet::SUCCESS) {
            SOCK_ERROR("Convert address error");
            return (-1);
        }
        return _send(&dest_addr, data, datalen, NULL, 0, 0);
    } else {
        return _send(NULL, data, datalen, NULL, 0, 0);
    }
}

/**
* @brief Recv - Recv message 
*
* @param [data] - Pointer that Space for receiving data
* @param [datalen] - Space length
*
* @returns  SockRet or recv length.
*/
ssize_t SockFD::Read(void* data, size_t datalen)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return (-1);
    }
    return _recv(NULL, data, datalen, NULL, 0, 0);
}

/**
* @brief Send - Send message,Generally used for UDP.
*               If not UDP, dest will not be used.
*
* @param [dest] - Destination address,if NULL same to other functions.
* @param [data] - Data pointer that will be sended.
* @param [datalen] - Data length
*
* @returns  SockRet or send length
*/
ssize_t SockFD::Send(SockAddress* dest_addr, const void* data, size_t datalen)
{
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return (-1);
    }
    SockAddress* tmp = isClientFD() ? &(this->dest) : &(this->orig);
    if (tmp->type_ == SOCK_DGRAM) {
        if (!dest_addr) {
            dest_addr = &(this->dest);
        }
        if (!dest_addr->AddrCheck()) {
            SOCK_ERROR("UDP send need destnation address");
            return (-1);
        }
        struct sockaddr s_dest_addr;
        memset(&s_dest_addr, 0x00, sizeof(struct sockaddr));
        if (dest_addr->ToSockaddr(&s_dest_addr) != SockRet::SUCCESS) {
            SOCK_ERROR("Convert address error");
            return (-1);
        }
        return _send(&s_dest_addr, data, datalen, NULL, 0, 0);
    } else {
        return _send(NULL, data, datalen, NULL, 0, 0);
    }
}

/**
* @brief Recv - Recv message,Generally used for UDP.
*               If not UDP, orig will not be used.
*
* @param [orig] - Source address.Allowed to be NULL.
* @param [data] - Pointer that Space for receiving data
* @param [datalen] - Space length
*
* @returns  SockRet or recv length.
*/
ssize_t SockFD::Recv(SockAddress* orig_addr, void* data, size_t datalen)
{
    int ret = 0;
    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return (-1);
    }
    SockAddress* tmp = isClientFD() ? &(this->dest) : &(this->orig);
    if (tmp->type_ == SOCK_DGRAM) {
        switch(tmp->domain_){
            case AF_LOCAL:
                struct sockaddr_un un_addr;
                ret = _recv((struct sockaddr*)&un_addr, data, datalen, NULL, 0, 0);
                if (orig_addr) {
                    orig_addr->_init(tmp->family_, un_addr.sun_path);
                }
                break;
            case AF_INET:
                struct sockaddr_in in_addr;
                ret = _recv((struct sockaddr*)&in_addr, data, datalen, NULL, 0, 0);
                if (orig_addr) {
                    orig_addr->_init(tmp->family_, inet_ntoa(in_addr.sin_addr), in_addr.sin_port);
                }
                break;
            case AF_INET6:
                char c_in6_addr[SOCK_ADDRESS_MAX_LEN];
                struct sockaddr_in6 in6_addr;
                ret = _recv((struct sockaddr*)&in6_addr, data, datalen, NULL, 0, 0);
               if (!inet_ntop(AF_INET6, &in6_addr.sin6_addr, c_in6_addr, SOCK_ADDRESS_MAX_LEN)) {
                    int temp_errno = errno;
                    SOCK_ERROR("%s%s", "Accept socket error, inet_ntop error", strerror(temp_errno));
                    return (-1);
               } 
               if (orig_addr) {
                   orig_addr->_init(tmp->family_, c_in6_addr, in6_addr.sin6_port);
                }
               break;
            default:
                SOCK_ERROR("Unknow socket family");
                return (-1);
        }
        return ret;
    } else {
        return _recv(NULL, data, datalen, NULL, 0, 0);
    }
}

/**
* @brief SendFD - Send a valid file descriptor to another process
*
* @param [fd] - File descriptor
*
* @returns  SockRet or send length
*/
ssize_t SockFD::SendFD(unsigned int fd)
{
    char ctrlmsg[CMSG_SPACE(sizeof(int))];

    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return (-1);
    }

    struct cmsghdr* pcmsg = reinterpret_cast<struct cmsghdr*>(ctrlmsg);
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS;
    *((int*)CMSG_DATA(pcmsg)) = fd;

    return _send(NULL, NULL, 0, ctrlmsg, sizeof(ctrlmsg), 0);
}

/**
* @brief RecvFD - Recv a Valid file descriptor from other process.
*
* @param [fd] - File descriptor
*
* @returns  SockRet or send length
*/
ssize_t SockFD::RecvFD(unsigned int* fd)
{
    int ret = 0;
    char ctrlmsg[CMSG_SPACE(sizeof(int))];

    if (!init_flag_) {
        SOCK_ERROR("%s", "fd not inited");
        return (-1);
    }

    struct cmsghdr* pcmsg = reinterpret_cast<struct cmsghdr*>(ctrlmsg);
    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS;

    ret = _recv(NULL, NULL, 0, ctrlmsg, sizeof(ctrlmsg), 0);

    *fd = *((int*)CMSG_DATA(pcmsg));

    return ret;
}

//private
/**
* @brief _close - close fd
*
* @returns  0 or -1
*/
int SockFD::_close()
{
    SOCK_DEBUG("sockfd[%d] closed", fd_);
    return close(fd_);
}

ssize_t SockFD::_send(struct sockaddr* dest, const void* data, size_t datalen, void* ctrldata, size_t ctrldatalen, int flags)
{
    int ret = 0;
    int temp_errno;
    if (!data && !ctrldata) {
        SOCK_ERROR("%s", "No data needs to be sent");
        return (-1);
    }
    struct msghdr msg;
    memset(&msg, 0x00, sizeof(struct msghdr));

    //fill addr
    struct sockaddr dest_addr;
    memset(&dest_addr, 0x00, sizeof(struct sockaddr));
    if (dest) {
        memcpy(&dest_addr, dest, sizeof(struct sockaddr));
        msg.msg_name = &dest_addr;
        msg.msg_namelen = sizeof(struct sockaddr);
    } else {
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
    }

    //fill data
    char fakebuff[1];
    char* buff = NULL;
    size_t bufflen = 0;
    struct iovec iov[1];
    bool buff_free_flag = false;

    if (data) {
        buff = static_cast<char*>(alloc_.Malloc(datalen));
        if (!buff) {
            SOCK_ERROR("%s", "Malloc error");
            return (-1);
        }
        memset(buff, 0x00, datalen);
        memcpy(buff, data, datalen);
        bufflen = datalen;
        buff_free_flag = true;
    } else {
        buff = fakebuff;
        bufflen = 1;
        buff_free_flag = false;
    }
    iov[0].iov_base = buff;
    iov[0].iov_len = bufflen;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    //fill ctrl data
    if (ctrldata) {
        msg.msg_control = ctrldata;
        msg.msg_controllen = ctrldatalen;
    }

    ret = sendmsg(fd_, &msg, flags);

    if (buff_free_flag) {
        alloc_.Free(buff);
    }
    return ret;
}

ssize_t SockFD::_recv(struct sockaddr* orig, void* data, size_t datalen, void* ctrldata, size_t ctrldatalen, int flags)
{
    int ret = 0;
    int temp_errno;
    struct msghdr msg;
    char fakebuff[1];
    char* buff = NULL;
    size_t bufflen = 0;
    struct iovec iov[1];

    if (!data && !ctrldata) {
        SOCK_ERROR("%s", "No data buffer, bad arguments");
        return (-1);
    }

    memset(&msg, 0x00, sizeof(struct msghdr));
    if (data) {
        buff = static_cast<char*>(alloc_.Malloc(datalen));
        if (!buff) {
            SOCK_ERROR("%s", "Malloc error");
            return (-1);
        }
        memset(buff, 0x00, datalen);
        bufflen = datalen;
    } else {
        buff = fakebuff;
        bufflen = 1;
    }
    iov[0].iov_base = buff;
    iov[0].iov_len = bufflen;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    //fill ctrl data
    if (ctrldata) {
        msg.msg_control = ctrldata;
        msg.msg_controllen = ctrldatalen;
    }

    if (orig) {
        msg.msg_name = orig;
        msg.msg_namelen = 16;
    }

    ret = recvmsg(fd_, &msg, flags);
    if (ret <= 0) {
        return ret;
    }

    //fill data
    if (data) {
        memcpy(data, buff, datalen);
        alloc_.Free(buff);
    }

    return ret;
}

}//namespace infra::socket end
