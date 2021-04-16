#include "io_log.h"
#include "io_epoll_fd.h"

namespace infra::io {

EpollFD::EpollFD() : FD() { }

EpollFD::EpollFD(int flags)
{
    fd_ = epoll_create1(flags);
}

EpollFD::EpollFD(unsigned int fd, bool auto_close)
{
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        Log::Error(strerror(temp_errno));
    }   
}

EpollFD::EpollFD(EpollFD& other) : FD(other) { }

EpollFD::~EpollFD()
{
    if (Valid() && auto_close_) {
        close(fd_);
    }
}

//Inherited from class FD.
Return EpollFD::SetFD(unsigned int fd, bool auto_close)
{
    if (Valid() && auto_close_) {
        close(fd_);
        fd_ = 0;
    }
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        Log::Error(strerror(temp_errno));
        return temp_errno;
    }
    fd_ = fd;
    auto_close_ = auto_close;
    return Return::SUCCESS;
}

Return EpollFD::Dup(io::FD& new_fd)
{

}

io::FD* EpollFD::Clone()
{
    return NULL;
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


Return EpollFD::AddEvent(FD& fd, int events)
{
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(struct epoll_event));
    
    ep_event.events = events;
    ep_event.events |= EPOLLET;
    
    ep_event.data.fd = fd.GetFD();
    if (epoll_ctl(fd_, EPOLL_CTL_ADD, fd.GetFD(), &ep_event) == -1) {
        int tmp_errno = errno;
        Log::Error("Epoll add fd ", ep_event.data.fd, " error ", strerror(tmp_errno));
        return tmp_errno;
    }
    return Return::SUCCESS;
}

Return EpollFD::AddEvent(SelectEvent& event)
{
    return AddEvent(event.GetFd(), event.GetEvents());
}

Return EpollFD::ModEvent(FD& fd, int events)
{
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(struct epoll_event));

    ep_event.events = events;
    ep_event.events |= EPOLLET;

    ep_event.data.fd = fd.GetFD();
    if (epoll_ctl(fd_, EPOLL_CTL_MOD, fd.GetFD(), &ep_event) == -1) {
        int tmp_errno = errno;
        Log::Error("Epoll add fd error ", strerror(tmp_errno));
        return tmp_errno;
    }
    return Return::SUCCESS;
}

Return EpollFD::ModEvent(SelectEvent& event)
{
    return ModEvent(event.GetFd(), event.GetEvents());
}

Return EpollFD::DelEvent(FD& fd)
{
    if (epoll_ctl(fd_, EPOLL_CTL_DEL, fd.GetFD(), NULL) == -1) {
        int tmp_errno = errno;
        Log::Error("Epoll add fd error ", strerror(tmp_errno));
        return tmp_errno;
    }
    return Return::SUCCESS;
}

}
