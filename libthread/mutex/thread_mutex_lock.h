/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 04:02
 * @file     : thread_mutex_lock.h
 * @brief    : Thread level mutex lock header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __THREAD_MUTEX_LOCK_H__
#define __THREAD_MUTEX_LOCK_H__

#include <pthread.h>
#include "thread_return.h"

namespace thread::mutex {

/**
* @brief - Thread level mutex lock.
*/
class ThreadMutexLock {
public:
    ThreadMutexLock();
    ~ThreadMutexLock();

    /**
    * @brief lock - Get mutex lock.
    *
    * @returns ThreadRet. 
    */
    ThreadRet lock();
    /**
    * @brief unlock - Release mutex lock.
    *
    * @returns  ThreadRet.
    */
    ThreadRet unlock();
private:
    pthread_mutex_t mutex_;
    bool init_;

    //Don't allow copy
    ThreadMutexLock(const ThreadMutexLock&);
    ThreadMutexLock& operator = (const ThreadMutexLock&);
};

} //namepace mutex thread


#endif
