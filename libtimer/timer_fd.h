#ifndef __TIMER_FD_H__
#define __TIMER_FD_H__

#include "object.h"
#include "io_fd.h"

#include "timer_return.h"
#include "timer_time.h"

namespace infra::timer {

/**
* @brief - timer file descriptor
*/
class FD  : virtual public base::Object, public io::FD {
public:
    /**
    * @brief - timer file descriptor flag
    */
    enum Flag {
        Realtime = 0x0001,
        Monotonic = 0x0002,
        CloseExec = 0x0004,
        Nonblock = 0x0008,
        Relative = 0x0010,
        Absolute = 0x0020,
    };
public:
    FD();
    FD(int flag);
    FD(int flag, Time& trigger_time, Time& interval_time);
    FD(unsigned int fd, bool auto_close = false);
    FD(FD& other);
    ~FD();

    //Inherited from class io::FD.
    io::Return SetFD(unsigned int fd, bool auto_close);
    io::Return Dup(FD& new_fd);
    io::FD* Clone();
    void Close();
    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

    Time& GetTriggerTime();
    Time& GetIntervalTime();
    int GetTriggerCounts();

    Return Enable();
    Return Enable(int flag, Time& trigger_time, Time& interval_time);
    Return Disable();

private:
    bool first_start_ = true;
    int flag_ = 0;
    Time trigger_time_;
    Time interval_time_;
};

}

#endif
