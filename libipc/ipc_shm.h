/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-22 13:17
 * @file     : shm.h
 * @brief    : Base shm virtual class.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __SHM_H__
#define __SHM_H__

#include <string>
#include <sys/types.h>

#include "ipc_return.h"
#include "ipc_mode.h"
#include "shm_status.h"

namespace ipc::shm {

/**
* @brief - Top share memory virtual class
*/
class Shm {
public:
    /**
    * @brief Shm - Constructor.
    */
    Shm() {
        path_.clear();
        head_ = NULL;
        size_ = 0;
        status_ = ShmStatus::UNKNOW;
    }

    /**
    * @brief Shm - Constructor.
    *
    * @param [path] - Share memory key(path).
    */
    Shm(std::string path) {
        path_ = path;
        head_ = NULL;
        size_ = 0;
    }

    /**
    * @brief ~Shm - Destructor.
    */
    virtual ~Shm() { }

    std::string GetPath() {
        return path_;
    }

    size_t GetSize() {
        return size_;
    }

    /**
    * @brief Create - Create share memory.
    *
    * @param [size] - Share memory total size.
    * @param [mode] - Share memory access mode.
    *
    * @returns  IpcRet.
    */
    virtual IpcRet Create(size_t size, mode_t mode) { return IpcRet::ESUBCLASS; }

    /**
    * @brief Destroy - Destroy share memory.
    *
    * @returns  IpcRet.
    */
    virtual IpcRet Destroy() { return IpcRet::ESUBCLASS; }

    /**
    * @brief Open - Open a share memory.
    *
    * @param [mode] - Share memory read-write mode.
    *
    * @returns  IpcRet.
    */
    virtual IpcRet Open(IpcMode mode) { return IpcRet::ESUBCLASS; }

    /**
    * @brief Close - Close a share memory.
    *
    * @returns  IpcRet.
    */
    virtual IpcRet Close() { return IpcRet::ESUBCLASS; }

    /**
    * @brief Sync - Sync cache to share memory.
    *
    * @returns  
    */
    virtual IpcRet Sync() { return IpcRet::ESUBCLASS; }

    /**
    * @brief GetHeadPtr - Get share memory head pointer.
    *
    * @returns  IpcRet.
    */
    void* GetHeadPtr() {
        return head_;
    }
protected:
    std::string path_;  ///< Share memory key(path).
    size_t size_;       ///< Share memory size.
    void* head_;        ///< Share memory pointer.
    ShmStatus status_;  ///< Share memory status.

    Shm(const Shm& other) {
        path_ = other.path_;
        head_ = other.head_;
        size_ = other.size_;
    }
};

}

#endif
