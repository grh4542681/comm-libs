/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-22 13:43
 * @file     : shm_sysv.h
 * @brief    : SystemV share memory.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __SHM_SYSV_H__
#define __SHM_SYSV_H__

#include <sys/types.h>
#include <sys/shm.h>

#include "ipc_log.h"
#include "ipc_return.h"
#include "shm.h"

namespace infra::ipc::shm {

/**
* @brief - SystemV share memory.
*/
class ShmSysV : public Shm {
public:
    /**
    * @brief ShmSysV - Constructor.
    *
    * @param [path] - Share memory key.
    */
    ShmSysV(std::string path);

    /**
    * @brief ShmSysV - Constructor
    */
    ShmSysV();

    /**
    * @brief ~ShmSysV - Destructor.
    */
    ~ShmSysV();

    /**
    * @brief Create - Create share memory.
    *
    * @param [size] - Share memory size.
    * @param [mode] - Share memory access mode.
    *
    * @returns  IpcRet.
    */
    IpcRet Create(size_t size, mode_t mode);

    /**
    * @brief Destroy - Destroy share memory.
    *
    * @returns  IpcRet.
    */
    IpcRet Destroy();

    /**
    * @brief Open - Open share memory.
    *
    * @param [mode] - Share memory read-write mode.
    *
    * @returns  IpcRet.
    */
    IpcRet Open(IpcMode mode);

    /**
    * @brief Close - Close share memory.
    *
    * @returns  IpcRet.
    */
    IpcRet Close();

    /**
    * @brief Sync - Sync cache to share memory.(system v no use).
    *
    * @returns  IpcRet.
    */
    IpcRet Sync();

private:
    key_t key_; ///< SystemV ipc key.
    int shmid_; ///< SystemV shm id.

    ShmSysV(const ShmSysV& other);
};

}//namespace infra::ipc end

#endif
