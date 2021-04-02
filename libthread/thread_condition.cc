#include <chrono>
#include "thread_condition.h"

namespace infra::thread {

Condition::Condition() { }
Condition::~Condition() { }

Return Condition::Wait(::infra::timer::Time* timeout)
{
    std::unique_lock<std::mutex> lk(m);
    if (timeout) {
        cv.wait_for(lk, std::chrono::microseconds(timeout->GetTime(::infra::timer::Time::Unit::Millisecond)), [&]{return ready;});
    } else {
        cv.wait(lk, [&]{return ready;});
    }
    ready = false;
    lk.unlock();
    return Return::SUCCESS;
}

Return Condition::Notify()
{
    std::lock_guard<std::mutex> lk(m);
    ready = true;
    cv.notify_one();
    return Return::SUCCESS;
}

Return Condition::NotifyAll()
{
    std::lock_guard<std::mutex> lk(m);
    ready = true;
    cv.notify_all();
    return Return::SUCCESS;
}

}
