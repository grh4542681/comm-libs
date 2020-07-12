/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 04:04
 * @file     : thread_rw_lock.h
 * @brief    : Thread level read/write lock header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __THREAD_RW_LOCK_H__
#define __THREAD_RW_LOCK_H__

#include <time.h>
#include <pthread.h>
#include "thread_return.h"

namespace thread::mutex {

/**
* @brief - Thread level read/write lock.
*/
class ThreadRWLock {
public:
    ThreadRWLock();
    ~ThreadRWLock();

    /**
    * @brief setNonBlock - Set nonblock flag.
    *
    * @param [bool] - ture/false
    */
    void setNonBlock(bool);
    /**
    * @brief getNonBlock - Get nonblock flag.
    *
    * @returns  ture/false.
    */
    bool getNonBlock();

    /**
    * @brief RLock - Get read lock.
    *
    * @param [overtime] - Over time.
    *
    * @returns  ThreadRet.
    */
    ThreadRet RLock(struct timespec* overtime);
    /**
    * @brief WLock - Get write lock.
    *
    * @param [overtime] - Over time.
    *
    * @returns  ThreadRet.
    */
    ThreadRet WLock(struct timespec* overtime);
    /**
    * @brief UnLock - Release lock.
    *
    * @returns  ThreadRet.
    */
    ThreadRet UnLock();

private:
    bool init_flag_;            ///< init flag, it must be ture when use this instance.
    bool nonblock_flag_;        ///< nonblock flag.
    pthread_rwlock_t rwlock_;   ///< package pthread lib rwlock.

    static int _time_compare(struct timespec* a, struct timespec* b) {
        if (a->tv_sec < b->tv_sec) {
            return -1;
        } else if (a->tv_sec > b->tv_sec) {
            return 1;
        } else {
            if (a->tv_nsec < b->tv_nsec) {
                return -1;
            } else if (a->tv_nsec > b->tv_nsec) {
                return 1;
            } else {
                return 0;
            }
        }
    }

    static ThreadRet _errno2ret(int ierrno) {
        switch (ierrno) {
            case 0:
                return ThreadRet::SUCCESS;
            case EBUSY:
            case EINVAL:
            case EAGAIN:
            case EDEADLK:
            default:
                return ThreadRet::ERROR;
        }
    }

};

} // namespace thread


#endif
