#include "mempool.h"

#include "ipc_log.h"
#include "event_fd.h"

namespace ipc::event {

EventFD::EventFD() : FD() { }

EventFD::EventFD(int initval, int flag) : FD()
{
    flag_ = flag;
    int flags = 0;
    if (flag_ & Flag::CloseExec) {
        flags |= EFD_CLOEXEC;
    }
    if (flag_ & Flag::Nonblock) {
        flags |= EFD_NONBLOCK;
    }
    if (flag_ & Flag::Semaphore) {
        flags |= EFD_SEMAPHORE;
    }
    fd_ = eventfd(initval, flags);
    if (fd_ < 0) {
        IPC_ERROR("Create event fd error : %s", strerror(errno));
        init_flag_ = false;
    }
    init_flag_ = true;
}

EventFD::EventFD(unsigned int fd, bool auto_close) : FD(fd, auto_close) { }

EventFD::EventFD(EventFD& other) : FD(other)
{
    flag_ = other.flag_;
}

EventFD::~EventFD()
{
    if (init_flag_ && auto_close_) {
        close(fd_);
    }
}

ret::Return EventFD::SetFD(unsigned int fd, bool auto_close)
{
    if (fd_ > 0 && init_flag_ && auto_close_) {
        close(fd_);
        fd_ = 0;
    }   
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        IPC_ERROR("%s", strerror(temp_errno));
        init_flag_ = false;
        return temp_errno;
    }   
    fd_ = fd; 
    init_flag_ = true;
    auto_close_ = auto_close;
    return IpcRet::SUCCESS;
}

ret::Return EventFD::Dup(io::FD& new_fd)
{
    return IpcRet::SUCCESS;
}

io::FD* EventFD::Clone()
{
    return mempool::Mempool::getInstance()->Malloc<EventFD>(*this);
}

void EventFD::Close()
{
    close(fd_);
}

ssize_t EventFD::Write(const void* data, size_t datalen)
{
    return write(fd_, data, datalen);
}

ssize_t EventFD::Read(void* data, size_t datalen)
{
    return read(fd_, data, datalen);
}

IpcRet EventFD::Increase(unsigned int count)
{
    if (Write(&count, sizeof(unsigned int)) < 0) {
        return IpcRet::EVT_EWRITE;
    }
}

IpcRet EventFD::Decrease()
{
    unsigned int count;
    return Decrease(&count);
}

IpcRet EventFD::Decrease(unsigned int* count)
{
    if (Read(count, sizeof(unsigned int)) < 0) {
        return IpcRet::EVT_EREAD;
    }
}

}
