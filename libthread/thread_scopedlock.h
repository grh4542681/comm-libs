#ifndef __THREAD_SCOPEDLOCK__
#define __THREAD_SCOPEDLOCK__

#include "object.h"

#include "thread_mutex.h"

namespace infra::thread {

class ScopedLock : virtual public ::infra::base::Object {
public:
    explicit ScopedLock(Mutex& mutex) : _mutex(mutex) {
        _mutex.Lock();
    }
    ~ScopedLock() {
        _mutex.Unlock();
    }
private:
    Mutex& _mutex;

    ScopedLock();
    ScopedLock(const ScopedLock&);
    ScopedLock& operator = (const ScopedLock&);
};

}

#endif
