#include "heartbeat_item.h"

namespace heartbeat {

HeartbeatItem::HeartbeatItem(io::FD& fd, timer::Time& over_time)
{
    fd_ = fd.Clone();
    if (!fd_) {
        state_ = HeartbeatState::INVALID;
        return;
    }
    mempool_ = mempool::Mempool::getInstance();
    over_time_ = over_time;
    dead_time_ = over_time;
    over_callback_ = NULL;
    dead_callback_ = NULL;
}

HeartbeatItem::~HeartbeatItem()
{
    if (fd_) {
        mempool_->Free<io::FD>(fd_);
    }
}

io::FD* HeartbeatItem::GetFD()
{
    return fd_;
}

HeartbeatState HeartbeatItem::GetState()
{
    return state_;
}

timer::Time& HeartbeatItem::GetLastTime()
{
    return last_time_;
}

timer::Time& HeartbeatItem::GetOverTime()
{
    return over_time_;
}

timer::Time& HeartbeatItem::GetDeadTime()
{
    return dead_time_;
}

HeartbeatItem& HeartbeatItem::SetLastTime(timer::Time& last_time)
{
    last_time_ = last_time;
    return *this;
}

HeartbeatItem& HeartbeatItem::SetOverTime(timer::Time& over_time)
{
    over_time_ = over_time;
    return *this;
}

HeartbeatItem& HeartbeatItem::SetDeadTime(timer::Time& dead_time)
{
    dead_time_ = dead_time;
    return *this;
}

}
