#include <pthread.h>
#include <errno.h>

#include "thread_log.h"
#include "thread_mutex.h"

namespace infra::thread {

Mutex::Mutex()
{
    this->init_flag_ = false;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    if (pthread_mutex_init(&(this->mutex_), &attr)) {
        Log::Error("Cannot create the mutex");
    } else {
        this->init_flag_ = true;
    }    
    pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex()
{
    if (this->init_flag_) {
        pthread_mutex_destroy(&(this->mutex_));
    }
}

Return Mutex::Lock(::infra::timer::Time* timeout)
{
    if (!this->init_flag_) {
        return Return::EINIT;
    }
    int rerrno = 0;
    if (!timeout) {
        if ((rerrno = pthread_mutex_lock(&(this->mutex_))) !=0 ) {
            Log::Error("Cannot lock the mutex, errno:", rerrno);
            return Return(rerrno);
        }
    } else if (timeout->IsZero()) {
        if ((rerrno = pthread_mutex_trylock(&(this->mutex_))) !=0 ) {
            Log::Error("Cannot lock the mutex, errno:", rerrno);
            return Return(rerrno);
        }
    } else {
        struct timespec ts;
        ::infra::timer::Time expect_ftime = ::infra::timer::Time::Now() + *timeout;
        expect_ftime.To(&ts);
        if ((rerrno = pthread_mutex_timedlock(&(this->mutex_), &ts)) !=0 ) {
            Log::Error("Cannot lock the mutex, errno:", rerrno);
            return Return(rerrno);
        }
    }
    return Return::SUCCESS;
}

Return Mutex::Unlock()
{
    if (!this->init_flag_) {
        return Return::EINIT;
    }
    if (int ret = pthread_mutex_unlock(&(this->mutex_))) {
        Log::Error("Cannot unlock the mutex, ret:", ret, "errno:", errno);
        return Return::ERROR;
    }
    return Return::SUCCESS;
}

} //namespace end
