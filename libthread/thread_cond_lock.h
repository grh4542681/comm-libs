#ifndef __THREAD_COND_LOCK_H__
#define __THREAD_COND_LOCK_H__

#include <pthread.h>

namespace thread::mutex {

class ThreadCondLock {
public:
    ThreadCondLock();
    ~ThreadCondLock();

    ThreadRet Wait();
    ThreadRet Signal();

private:


}

}

#endif
