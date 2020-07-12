/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 09:08
 * @file     : thread_rw_lock.cc
 * @brief    : Thread level read/write lock.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#include <string.h>
#include <time.h>

#include "thread_log.h"
#include "thread_rw_lock.h"

namespace thread::mutex {

ThreadRWLock::ThreadRWLock()
{
    this->init_flag_ = false;
    if (pthread_rwlock_init(&(this->rwlock_), NULL)) {
        THREAD_ERROR("RWlock init error[%s]", strerror(errno));
        return;
    }
    this->init_flag_ = true;
    this->nonblock_flag_ = false;
}

ThreadRWLock::~ThreadRWLock()
{
    if (this->init_flag_) {
        pthread_rwlock_destroy(&(this->rwlock_));
    }
}

void ThreadRWLock::setNonBlock(bool flag)
{
    this->nonblock_flag_ = flag;
}

bool ThreadRWLock::getNonBlock()
{
    return this->nonblock_flag_;
}

ThreadRet ThreadRWLock::RLock(struct timespec* overtime)
{
    int ret;
    if (this->nonblock_flag_) {
        if ((ret = pthread_rwlock_tryrdlock(&(this->rwlock_))) != 0)
        {
            THREAD_ERROR("Locked failed [%s]", strerror(ret));
            return ThreadRet::ERROR;
        }
    } else {
        if (!overtime) {
            if ((ret = pthread_rwlock_rdlock(&(this->rwlock_))) != 0) {
                THREAD_ERROR("Locked failed [%s]", strerror(ret));
                return _errno2ret(ret);
            }
        } else {
            struct timespec curr_time;
            struct timespec finish_time;
            memset(&curr_time, 0x00, sizeof(struct timespec));
            memset(&finish_time, 0x00, sizeof(struct timespec));
            finish_time.tv_sec = curr_time.tv_sec + overtime->tv_sec;
            finish_time.tv_nsec = curr_time.tv_nsec + overtime->tv_nsec;

            do {
                clock_gettime(CLOCK_REALTIME, &curr_time);
                ret = pthread_rwlock_timedrdlock(&(this->rwlock_), overtime);
            } while ( ret == EAGAIN && _time_compare(&curr_time, &finish_time) < 0);

            if (ret) {
                THREAD_ERROR("Locked failed [%s]", strerror(ret));
                return _errno2ret(ret);
            }
        }
    }
    return ThreadRet::SUCCESS;
}

ThreadRet ThreadRWLock::WLock(struct timespec* overtime)
{
    int ret;
    if (this->nonblock_flag_) {
        if ((ret = pthread_rwlock_trywrlock(&(this->rwlock_))) != 0)
        {
            THREAD_ERROR("Locked failed [%s]", strerror(ret));
            return _errno2ret(ret);
        }
    } else {
        if (!overtime) {
            if ((ret = pthread_rwlock_wrlock(&(this->rwlock_))) != 0) {
                THREAD_ERROR("Locked failed [%s]", strerror(ret));
                return _errno2ret(ret);
            }
        } else {
            struct timespec curr_time;
            struct timespec finish_time;
            memset(&curr_time, 0x00, sizeof(struct timespec));
            memset(&finish_time, 0x00, sizeof(struct timespec));
            finish_time.tv_sec = curr_time.tv_sec + overtime->tv_sec;
            finish_time.tv_nsec = curr_time.tv_nsec + overtime->tv_nsec;

            do {
                clock_gettime(CLOCK_REALTIME, &curr_time);
                ret = pthread_rwlock_timedwrlock(&(this->rwlock_), overtime);
            } while ( ret == EAGAIN && _time_compare(&curr_time, &finish_time) < 0);

            if (ret) {
                THREAD_ERROR("Locked failed [%s]", strerror(ret));
                return _errno2ret(ret);
            }
        }
    }
    return ThreadRet::SUCCESS;
}

ThreadRet ThreadRWLock::UnLock()
{
    int ret;
    if ((ret = pthread_rwlock_unlock(&(this->rwlock_))) != 0) {
        THREAD_ERROR("Unlocked failed [%s]", strerror(ret));
        return _errno2ret(ret);
    }
    return ThreadRet::SUCCESS;
}

} //namespace end
