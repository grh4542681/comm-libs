#ifndef __THREAD_CONDITION_H__
#define __THREAD_CONDITION_H__

#include <thread>
#include <mutex>
#include <condition_variable>

#include "object.h"
#include "timer_time.h"

#include "thread_return.h"

namespace infra::thread {

class Condition : virtual public ::infra::base::Object {
public:
    Condition();
    ~Condition();

    Return Wait(::infra::timer::Time* timeout = NULL);
    Return Notify();
    Return NotifyAll();

private:
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
};

}

#endif
