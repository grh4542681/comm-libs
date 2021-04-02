#ifndef __THREAD_EVENT_H__
#define __THREAD_EVENT_H__

#include "object.h"
#include "io_fd.h"

#include "thread_return.h"
#include "thread_mutex.h"

namespace infra::thread {

class Event : virtual public ::infra::base::Object, private io::FD {
public:
    Event();
    Event(unsigned int fd, bool auto_close = false);
    Event(Event& other);
    ~Event();

    Return Send(uint64_t event);
    Return Recv(uint64_t* event);
private:
    //Inherited from class FD.
    io::Return SetFD(unsigned int fd, bool auto_close);
    io::Return Dup(io::FD& new_fd);
    io::FD* Clone();
    void Close();
    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

private:
    Mutex send_mutex_;
    Mutex recv_mutex_;
};

}

#endif
