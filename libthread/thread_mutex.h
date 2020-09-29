#ifndef __THREAD_MUTEX_H__
#define __THREAD_MUTEX_H__

#include <pthread.h>

#include "object.h"
#include "timer_time.h"

#include "thread_return.h"

namespace infra::thread {

/**
* @brief - Thread level mutex lock.
*/
class Mutex : virtual public ::infra::base::Object {
public:
    Mutex();
    ~Mutex();

    Return Lock(::infra::timer::Time* timeout = NULL);
    Return Unlock();
private:
    pthread_mutex_t mutex_;
    bool init_flag_;

    //Don't allow copy
    Mutex(const Mutex&);
    Mutex& operator = (const Mutex&);
};

} //namepace mutex thread


#endif
