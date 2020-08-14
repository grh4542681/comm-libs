#include "io_log.h"
#include "io_epoll_fd.h"

namespace infra::io {

EpollFD::EpollFD() : FD() { }

EpollFD::EpollFD(int flags)
{
    fd_ = epoll_create1(flags);
    if (fd_ == -1) {
        init_flag_ = false;
    } else {
        init_flag_ = true;
    }
}

EpollFD::EpollFD(unsigned int fd, bool auto_close)
{
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        IO_ERROR("%s", strerror(temp_errno));
        init_flag_ = false;
    }   
    init_flag_ = true;
}

EpollFD::EpollFD(EpollFD& other) : FD(other) { }

EpollFD::~EpollFD()
{
    if (init_flag_ && auto_close_) {
        close(fd_);
    }
}

//Inherited from class FD.
ret::Return EpollFD::SetFD(unsigned int fd, bool auto_close)
{
    if (fd_ > 0 && init_flag_ && auto_close_) {
        close(fd_);
        fd_ = 0;
    }
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        IO_ERROR("%s", strerror(temp_errno));
        init_flag_ = false;
        return temp_errno;
    }
    fd_ = fd;
    init_flag_ = true;
    auto_close_ = auto_close;
    return ret::Return::SUCCESS;
}

ret::Return EpollFD::Dup(io::FD& new_fd)
{

}

io::FD* EpollFD::Clone()
{
    return alloc_.Allocate<EpollFD>(*this);
}

void EpollFD::Close()
{
    close(fd_);
}

ssize_t EpollFD::Write(const void* data, size_t datalen)
{
    return -1;
}

ssize_t EpollFD::Read(void* data, size_t datalen)
{
    return -1;
}


IoRet EpollFD::AddEvent(FD& fd, int events)
{
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(struct epoll_event));
    
    ep_event.events = events;
    ep_event.events |= EPOLLET;
    
    ep_event.data.fd = fd.GetFD();
    if (epoll_ctl(fd_, EPOLL_CTL_ADD, fd.GetFD(), &ep_event) == -1) {
        int tmp_errno = errno;
        IO_ERROR("Epoll add fd[%d] error %s", ep_event.data.fd, strerror(tmp_errno));
        return tmp_errno;
    }
    return IoRet::SUCCESS;
}

IoRet EpollFD::AddEvent(SelectEvent& event)
{
    return AddEvent(event.GetFd(), event.GetEvents());
}

IoRet EpollFD::ModEvent(FD& fd, int events)
{
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(struct epoll_event));

    ep_event.events = events;
    ep_event.events |= EPOLLET;

    ep_event.data.fd = fd.GetFD();
    if (epoll_ctl(fd_, EPOLL_CTL_MOD, fd.GetFD(), &ep_event) == -1) {
        int tmp_errno = errno;
        IO_ERROR("Epoll add fd error %s", strerror(tmp_errno));
        return tmp_errno;
    }
    return IoRet::SUCCESS;
}

IoRet EpollFD::ModEvent(SelectEvent& event)
{
    return ModEvent(event.GetFd(), event.GetEvents());
}

IoRet EpollFD::DelEvent(FD& fd)
{
    if (epoll_ctl(fd_, EPOLL_CTL_DEL, fd.GetFD(), NULL) == -1) {
        int tmp_errno = errno;
        IO_ERROR("Epoll add fd error %s", strerror(tmp_errno));
        return tmp_errno;
    }
    return IoRet::SUCCESS;
}

}
