#include "thread_log.h"
#include "thread_rwlock.h"

namespace infra::thread {

RwLock::RwLock()
{
    this->init_flag_ = false;
    if (pthread_rwlock_init(&(this->rwlock_), NULL)) {
        Log::Error("RWlock init error[%s]", strerror(errno));
        return;
    }
    this->init_flag_ = true;
}

RwLock::~RwLock()
{
    if (this->init_flag_) {
        pthread_rwlock_destroy(&(this->rwlock_));
    }
}

Return RwLock::RLock(::infra::timer::Time* timeout)
{
    int rerrno = 0;
    if (!timeout) {
        if ((rerrno = pthread_rwlock_rdlock(&(this->rwlock_))) != 0) {
            Log::Error("Locked read failed [", strerror(rerrno), "]");
            return Return(rerrno);
        }
    } else if (timeout->IsZero()) {
        if ((rerrno = pthread_rwlock_tryrdlock(&(this->rwlock_))) != 0) {
            Log::Error("Locked read failed [", strerror(rerrno), "]");
            return Return(rerrno);
        }
    } else {
        struct timespec ts;
        ::infra::timer::Time expect_ftime = ::infra::timer::Time::Now() + *timeout;
        expect_ftime.To(&ts);
        if ((rerrno = pthread_rwlock_timedrdlock(&(this->rwlock_), &ts)) !=0 ) {
            Log::Error("Locked read failed [", strerror(rerrno), "]");
            return Return(rerrno);
        }
    }
    return Return::SUCCESS;
}

Return RwLock::WLock(::infra::timer::Time* timeout)
{
    int rerrno = 0;
    if (!timeout) {
        if ((rerrno = pthread_rwlock_wrlock(&(this->rwlock_))) != 0) {
            Log::Error("Locked write failed [", strerror(rerrno), "]");
            return Return(rerrno);
        }
    } else if (timeout->IsZero()) {
        if ((rerrno = pthread_rwlock_trywrlock(&(this->rwlock_))) != 0) {
            Log::Error("Locked write failed [", strerror(rerrno), "]");
            return Return(rerrno);
        }
    } else {
        struct timespec ts;
        ::infra::timer::Time expect_ftime = ::infra::timer::Time::Now() + *timeout;
        expect_ftime.To(&ts);
        if ((rerrno = pthread_rwlock_timedwrlock(&(this->rwlock_), &ts)) !=0 ) {
            Log::Error("Locked write failed [", strerror(rerrno), "]");
            return Return(rerrno);
        }
    }
    return Return::SUCCESS;
}

Return RwLock::UnLock()
{
    int rerrno;
    if ((rerrno = pthread_rwlock_unlock(&(this->rwlock_))) != 0) {
        Log::Error("Locked failed [", strerror(rerrno), "]");
        return Return(rerrno);
    }
    return Return::SUCCESS;
}

} //namespace end
