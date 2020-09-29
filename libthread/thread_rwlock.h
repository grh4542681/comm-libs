#ifndef __THREAD_RWLOCK_H__
#define __THREAD_RWLOCK_H__

#include <pthread.h>

#include "timer_time.h"

#include "thread_return.h"

namespace infra::thread{

/**
* @brief - Thread level read/write lock.
*/
class RwLock {
public:
    RwLock();
    ~RwLock();

    Return RLock(::infra::timer::Time* timeout = NULL);
    Return WLock(::infra::timer::Time* timeout = NULL);
    Return UnLock();

private:
    bool init_flag_;            ///< init flag, it must be ture when use this instance.
    pthread_rwlock_t rwlock_;   ///< package pthread lib rwlock.
};

} // namespace thread


#endif
