#ifndef __TIMER_FD_H__
#define __TIMER_FD_H__

#include "io_fd.h"

#include "timer_return.h"
#include "timer_time.h"

namespace timer {

class TimerFD : public io::FD {
public:
    enum Flag {
        Realtime = 0x0001,
        Monotonic = 0x0002,
        CloseExec = 0x0004,
        Nonblock = 0x0008,
        Relative = 0x0010,
        Absolute = 0x0020,
    };
public:
    TimerFD();
    TimerFD(int flag);
    TimerFD(int flag, Time& trigger_time, Time& interval_time);
    TimerFD(unsigned int fd, bool auto_close = false);
    TimerFD(TimerFD& other);
    ~TimerFD();

    //Inherited from class FD.
    base::Return SetFD(unsigned int fd, bool auto_close);
    base::Return Dup(io::FD& new_fd);
    io::FD* Clone();
    void Close();
    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

    Time& GetTriggerTime();
    Time& GetIntervalTime();
    int GetTriggerCounts();

    Return Start();
    Return Start(int flag, Time& trigger_time, Time& interval_time);
    Return Stop();

private:
    bool first_start_ = true;
    int flag_ = 0;
    Time trigger_time_;
    Time interval_time_;
};

}

#endif
