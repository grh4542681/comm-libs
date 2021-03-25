#ifndef __HEARTBEAT_ITEM_H__
#define __HEARTBEAT_ITEM_H__

#include "io_fd.h"
#include "time/timer_time.h"
#include "mempool.h"

#include "heartbeat_state.h"

namespace infra::heartbeat {

class HeartbeatItem {
public:
    HeartbeatItem(io::FD& fd, timer::Time& over_time);
    ~HeartbeatItem();

    io::FD* GetFD();
    HeartbeatState GetState();

    timer::Time& GetLastTime();
    timer::Time& GetOverTime();
    timer::Time& GetDeadTime();

    HeartbeatItem& SetLastTime(timer::Time& last_time);
    HeartbeatItem& SetOverTime(timer::Time& over_time);
    HeartbeatItem& SetDeadTime(timer::Time& dead_time);

private:
    mempool::Mempool* mempool_;
    io::FD* fd_;
    HeartbeatState state_;

    timer::Time last_time_;
    timer::Time over_time_;
    timer::Time dead_time_;

    void (*over_callback_)(void);
    void (*dead_callback_)(void);
};

}

#endif
