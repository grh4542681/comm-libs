#include <sys/timerfd.h>

#include "timer_fd.h"
#include "timer_logger.h"

namespace timer {

TimerFD::TimerFD() : FD()
{

}

TimerFD::TimerFD(int flag)
{
    first_start_ = true;
    auto_close_ = false;
    flag_ = flag;

    int clockid = 0;
    int flags = 0;

    if (flag_ & Flag::Realtime) {
        clockid |= CLOCK_REALTIME;
    }
    if (flag_ & Flag::Monotonic) {
        clockid |= CLOCK_MONOTONIC;
    }
    if (flag_ & Flag::CloseExec) {
        flags |= TFD_CLOEXEC;
    }
    if (flag_ & Flag::Nonblock) {
        flags |= TFD_NONBLOCK;
    }

    fd_ = timerfd_create(clockid, flags);
    if (fd_ <= 0) {
        TIMER_ERROR("Create timer fd error: %s", strerror(errno));
        init_flag_ = false;
    }
    init_flag_ = true;
}

TimerFD::TimerFD(int flag, Time& trigger_time, Time& interval_time)
{
    first_start_ = true;
    auto_close_ = false;
    flag_ = flag;
    trigger_time_ = trigger_time;
    interval_time_ = interval_time;

    int clockid = 0;
    int flags = 0;

    if (flag_ & Flag::Realtime) {
        clockid |= CLOCK_REALTIME;
    }
    if (flag_ & Flag::Monotonic) {
        clockid |= CLOCK_MONOTONIC;
    }
    if (flag_ & Flag::CloseExec) {
        flags |= TFD_CLOEXEC;
    }
    if (flag_ & Flag::Nonblock) {
        flags |= TFD_NONBLOCK;
    }

    fd_ = timerfd_create(clockid, flags);
    if (fd_ <= 0) {
        TIMER_ERROR("Create timer fd error: %s", strerror(errno));
        init_flag_ = false;
    }
    init_flag_ = true;
}

TimerFD::TimerFD(unsigned int fd, bool auto_close) : FD(fd, auto_close)
{
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        TIMER_ERROR("%s", strerror(temp_errno));
        init_flag_ = false;
    }
    init_flag_ = true;
    first_start_ = true;
}

TimerFD::TimerFD(TimerFD& other) : FD(other)
{
    first_start_  = other.first_start_;
    flag_ = other.flag_;
    trigger_time_ = other.trigger_time_;
    interval_time_ = other.interval_time_;
}

TimerFD::~TimerFD()
{
    if (init_flag_ && auto_close_) {
        close(fd_);
    }
}

base::Return TimerFD::SetFD(unsigned int fd, bool auto_close)
{
    if (fd_ > 0 && init_flag_ && auto_close_) {
        close(fd_);
        fd_ = 0;
    }
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        TIMER_ERROR("%s", strerror(temp_errno));
        init_flag_ = false;
        return temp_errno;
    }
    fd_ = fd;
    init_flag_ = true;
    auto_close_ = auto_close;
    first_start_ = true;
    trigger_time_.SetTime(0, Time::Unit::Second);
    interval_time_.SetTime(0, Time::Unit::Second);
    return Return::SUCCESS;
}

base::Return TimerFD::Dup(io::FD& new_fd)
{
    return Return::SUCCESS;
}

io::FD* TimerFD::Clone()
{
    return alloc_.Allocate<TimerFD>(*this);
}

void TimerFD::Close()
{
    close(fd_);
}

ssize_t TimerFD::Write(const void* data, size_t datalen)
{
    TIMER_ERROR("Timer fd not support write.");
    return 0;
}

ssize_t TimerFD::Read(void* data, size_t datalen)
{
    return read(fd_, data, datalen);
}

Time& TimerFD::GetTriggerTime()
{
    return trigger_time_;
}

Time& TimerFD::GetIntervalTime()
{
    return interval_time_;
}

int TimerFD::GetTriggerCounts()
{
    return 0;
}

Return TimerFD::Start(int flag, Time& trigger_time, Time& interval_time)
{
    flag_ = flag;
    trigger_time_ = trigger_time;
    interval_time_ = interval_time;
    first_start_ = true;
    return Start();
}

Return TimerFD::Start()
{
    int flags = 0;
    if (flag_ & Flag::Absolute) {
        flags |= TFD_TIMER_ABSTIME;
    }

    struct itimerspec itime;
    memset(&itime, 0x00, sizeof(struct itimerspec));

    if (first_start_) {
        first_start_ = false;
        trigger_time_.To(&itime.it_value);
    }
    interval_time_.To(&itime.it_interval);

    if (timerfd_settime(fd_, flags, &itime, NULL) < 0) {
        int tmp_errno = errno;
        TIMER_ERROR("Start timer error: %s", strerror(tmp_errno));
        return tmp_errno;
    }
    return Return::SUCCESS;
}

Return TimerFD::Stop()
{
    int flags = 0;
    if (flag_ & Flag::Absolute) {
        flags |= TFD_TIMER_ABSTIME;
    }

    struct itimerspec itime;
    memset(&itime, 0x00, sizeof(struct itimerspec));
    
    if (timerfd_settime(fd_, flags, &itime, NULL) < 0) {
        int tmp_errno = errno;
        TIMER_ERROR("Stop timer error: %s", strerror(tmp_errno));
        return tmp_errno;
    }
    return Return::SUCCESS;
}


}
