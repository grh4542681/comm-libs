#include <sys/timerfd.h>

#include "timer_fd.h"
#include "timer_log.h"

namespace infra::timer {

FD::FD() : io::FD()
{

}

FD::FD(int flag)
{
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
        Log::Error("create timer-fd error: ", strerror(errno));
        fd_ = -1;
    }
}

FD::FD(int flag, Time& trigger_time, Time& interval_time)
{
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
        Log::Error("create timer-fd error: ", strerror(errno));
        fd_ = -1;
    }
}

FD::FD(unsigned int fd, bool auto_close) : io::FD(fd, auto_close)
{
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        Log::Error("create timer-fd error: ", strerror(temp_errno));
        fd = -1;
    }
}

FD::FD(FD& other) : io::FD(other)
{
    flag_ = other.flag_;
    trigger_time_ = other.trigger_time_;
    interval_time_ = other.interval_time_;
}

FD::~FD()
{
    if (Valid() && GetAutoClose()) {
        Close();
    }
}

io::Return FD::SetFD(unsigned int fd, bool auto_close)
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
    trigger_time_.SetTime(0, Time::Unit::Second);
    interval_time_.SetTime(0, Time::Unit::Second);
    return Return::SUCCESS;
}

io::Return FD::Dup(FD& new_fd)
{
    return Return::SUCCESS;
}

io::FD* FD::Clone()
{
    return new FD(*this);
}

void FD::Close()
{
    close(fd_);
}

ssize_t FD::Write(const void* data, size_t datalen)
{
    Log::Error("timer-fd not support write.");
    return 0;
}

ssize_t FD::Read(void* data, size_t datalen)
{
    return read(fd_, data, datalen);
}

Time& FD::GetTriggerTime()
{
    return trigger_time_;
}

Time& FD::GetIntervalTime()
{
    return interval_time_;
}

int FD::GetTriggerCounts()
{
    return 0;
}

Return FD::Enable(int flag, Time& trigger_time, Time& interval_time)
{
    flag_ = flag;
    trigger_time_ = trigger_time;
    interval_time_ = interval_time;
    return Enable();
}

Return FD::Enable()
{
    int flags = 0;
    if (flag_ & Flag::Absolute) {
        flags |= TFD_TIMER_ABSTIME;
    }

    struct itimerspec itime;
    memset(&itime, 0x00, sizeof(struct itimerspec));

    trigger_time_.To(&itime.it_value);
    interval_time_.To(&itime.it_interval);

    if (timerfd_settime(fd_, flags, &itime, NULL) < 0) {
        int tmp_errno = errno;
        Log::Error("enable timer-fd error: ", strerror(tmp_errno));
        return tmp_errno;
    }
    return Return::SUCCESS;
}

Return FD::Disable()
{
    int flags = 0;
    if (flag_ & Flag::Absolute) {
        flags |= TFD_TIMER_ABSTIME;
    }

    struct itimerspec itime;
    memset(&itime, 0x00, sizeof(struct itimerspec));
    
    if (timerfd_settime(fd_, flags, &itime, NULL) < 0) {
        int tmp_errno = errno;
        Log::Error("disable timer-fd error: ", strerror(tmp_errno));
        return tmp_errno;
    }
    return Return::SUCCESS;
}

}
