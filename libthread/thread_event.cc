#include <sys/eventfd.h>

#include "thread_log.h"
#include "thread_event.h"

namespace infra::thread {

Event::Event() : FD()
{
    fd_ = eventfd(0, EFD_CLOEXEC);
    if (fd_ <= 0) {
        Log::Error("create event-fd error: ", strerror(errno));
        fd_ = -1;
    }
}

Event::Event(unsigned int fd, bool auto_close) : FD(fd, auto_close) { }

Event::Event(Event& other) : FD(other) { }

Event::~Event()
{
    if (Valid() && GetAutoClose()) {
        Close();
    }
}

Return Event::Send(uint64_t event)
{
    send_mutex_.Lock();
    Write(&event, sizeof(uint64_t));
    send_mutex_.Unlock();

    return Return::SUCCESS;
}

Return Event::Recv(uint64_t* event)
{
    recv_mutex_.Lock();
    Read(event, sizeof(uint64_t));
    recv_mutex_.Unlock();

    return Return::SUCCESS;
}

//private
//Inherited from class FD.
io::Return Event::SetFD(unsigned int fd, bool auto_close)
{
    if (Valid()) {
        Close();
        fd_ = -1;
    }
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        Log::Error(strerror(temp_errno));
        fd = -1;
        auto_close_ = auto_close;
        return temp_errno;
    }
    fd_ = fd;
    auto_close_ = auto_close;
    return Return::SUCCESS;
}

io::Return Event::Dup(io::FD& new_fd)
{
    return Return::ERROR;
}

io::FD* Event::Clone()
{
    return nullptr;
}

void Event::Close()
{
    close(fd_);
}

ssize_t Event::Write(const void* data, size_t datalen)
{
    return write(fd_, data, datalen);
}

ssize_t Event::Read(void* data, size_t datalen)
{
    return read(fd_, data, datalen);
}

}
