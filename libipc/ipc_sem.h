/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-27 05:11
 * @file     : sem.h
 * @brief    : Base sem virtual class.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __SEM_H__
#define __SEM_H__

#include <sys/types.h>
#include <string>

#include "timer_time.h"
#include "ipc_return.h"
#include "ipc_mode.h"

namespace ipc::sem {

/**
* @brief - Top semphores virtual class.
*/
class Sem {
public:
    /**
    * @brief Sem - Constructor.
    *
    * @param [path] - Sem key(path).
    */
    Sem(std::string path) {
        path_ = path;
        nonblock_flag_ = false;
        init_flag_ = false;
    }
    /**
    * @brief Sem - Constructor.
    */
    Sem() {
        nonblock_flag_ = false;
        init_flag_ = false;
    }
    /**
    * @brief ~Sem - Deconstructor.
    */
    virtual ~Sem() { }

    virtual bool IsExist() { return false; }
    /**
    * @brief Create - Create semaphore set.
    *
    * @param [semnum] - Semaphore number in set.
    * @param [mode] - Semaphore set access mode.
    *
    * @returns  IpcRet.
    */
    virtual IpcRet Create(size_t semnum, mode_t mode) { return IpcRet::ESUBCLASS; }
    /**
    * @brief Destroy - Destroy semaphore set.
    *
    * @returns  IpcRet.
    */
    virtual IpcRet Destroy() { return IpcRet::ESUBCLASS; }
    /**
    * @brief Open - Open semaphore set.
    *
    * @param [mode] - Semaphore set read write mode
    *
    * @returns  IpcRet.
    */
    virtual IpcRet Open(IpcMode mode) { return IpcRet::ESUBCLASS; }
    /**
    * @brief Close - Close semaphore set.
    *
    * @returns  IpcRet.
    */
    virtual IpcRet Close() { return IpcRet::ESUBCLASS; }

    /**
    * @brief P - Grab semaphore(-n).
    *
    * @param [sem_index] - Semaphore index in set.
    * @param [num] - Grab semaphore value.
    * @param [overtime] - Overtime time.
    *
    * @returns  IpcRet.
    */
    IpcRet P(size_t sem_index, timer::Time* overtime) {
        return (init_flag_ ? _p(sem_index, overtime) : IpcRet::EINIT);
    }
    /**
    * @brief V - Release semaphore(+n).
    *
    * @param [sem_index] - Semaphore index in set.
    * @param [num] - Release semaphore value.
    *
    * @returns  IpcRet.
    */
    IpcRet V(size_t sem_index) {
        return (init_flag_ ? _v(sem_index) : IpcRet::EINIT);
    }

    /**
    * @brief SetNonBlock - Set semphore op non-block.
    *
    * @param [flag] - True or false;
    *
    * @returns  Current non-block flag.
    */
    bool SetNonBlock(bool flag) {
        nonblock_flag_ = flag;
        return this->nonblock_flag_;
    }

protected:
    std::string path_;      ///< Semaphore key(path).
    size_t semnum_;         ///< Semaphore numbers in set.
    bool nonblock_flag_;    ///< Nonblock flag.
    bool init_flag_;        ///< Initliazation flag.

    Sem(const Sem& other) {
        path_ = other.path_;
        nonblock_flag_ = other.nonblock_flag_;
        init_flag_ = false;
    }

    virtual IpcRet _p(size_t sem_index, timer::Time* overtime) { return IpcRet::ESUBCLASS; }
    virtual IpcRet _v(size_t sem_index) { return IpcRet::ESUBCLASS; }
};

}

#endif
