/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-22 13:48
 * @file     : shm_sysv.cc
 * @brief    : SystemV share memory.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#include <string.h>

#include "ipc_log.h"
#include "shm_sysv.h"

namespace ipc::shm {

ShmSysV::ShmSysV(std::string path) : Shm(path)
{
    key_ = ftok(path.c_str(), 100);
    shmid_ = -1;
}

ShmSysV::ShmSysV() : Shm()
{
    key_ = 0;
    shmid_ = -1;
}

ShmSysV::ShmSysV(const ShmSysV& other) : Shm(other)
{
    key_ = other.key_;
    shmid_ = other.shmid_;
}

ShmSysV::~ShmSysV()
{
    if (shmid_ > 0) {
        Close();
    }
}

IpcRet ShmSysV::Create(size_t size, mode_t mode)
{
    if (shmid_ > 0) {
        return IpcRet::SHM_ECREATED;
    }
    if (path_.empty() || size <= 0) {
        return IpcRet::EINIT;
    }
    shmid_ = shmget(key_, size, mode|IPC_CREAT|IPC_EXCL);
    if (shmid_ < 0) {
        int tmp_errno = errno;
        IPC_ERROR("Create sysv share memory error [%s]", strerror(tmp_errno));
        return tmp_errno;
    }
    struct shmid_ds shm_info;
    memset(&shm_info, 0, sizeof(struct shmid_ds));
    if (shmctl(shmid_, IPC_INFO, &shm_info) < 0) {
        int tmp_errno = errno;
        return tmp_errno;
    }
    size_ = shm_info.shm_segsz;
    status_ = ShmStatus::OPEN;
    return IpcRet::SUCCESS;
}

IpcRet ShmSysV::Destroy()
{
    if (shmid_ < 0) {
        shmid_ = shmget(key_, 0, 0);
        if (shmid_ < 0) {
            int tmp_errno = errno;
            return tmp_errno;
        }
    }
    if (shmctl(shmid_, IPC_RMID, NULL) < 0) {
        int tmp_errno = errno;
        return tmp_errno;
    }
    status_ = ShmStatus::DESTROY;
    return IpcRet::SUCCESS;
}

IpcRet ShmSysV::Open(IpcMode mode)
{
    if (path_.empty()) {
        return IpcRet::EINIT;
    }
    if (shmid_ < 0) {
        shmid_ = shmget(key_, 0, 0);
        if (shmid_ < 0) {
            int tmp_errno = errno;
            return tmp_errno;
        }
    }
    struct shmid_ds shm_info;
    memset(&shm_info, 0, sizeof(struct shmid_ds));
    if (shmctl(shmid_, IPC_STAT, &shm_info) < 0) {
        int tmp_errno = errno;
        return tmp_errno;
    }
    size_ = shm_info.shm_segsz;
    if (mode & IpcMode::READ_ONLY) {
        head_ = shmat(shmid_, NULL, SHM_RDONLY);
        if (head_ == (void*)-1) {
            int tmp_errno = errno;
            head_ = NULL;
            return tmp_errno;
        }
    } else if ((mode & IpcMode::WRITE_ONLY) || (mode & IpcMode::READ_WRITE)) {
        head_ = shmat(shmid_, NULL, 0);
        if (head_ == (void *)-1) {
            int tmp_errno = errno;
            head_ = NULL;
            return tmp_errno;
        }
    } else {
        return IpcRet::SHM_EMODE;
    }
    status_ = ShmStatus::OPEN;
    return IpcRet::SUCCESS;
}

IpcRet ShmSysV::Close()
{
    if (shmid_ < 0 || head_ == NULL) {
        return IpcRet::EINIT;
    }
    if (shmdt(head_) == -1) {
        int tmp_errno = errno;
        return tmp_errno;
    }
    head_ = NULL;
    status_ = ShmStatus::CLOSE;
    return IpcRet::SUCCESS;
}

IpcRet ShmSysV::Sync()
{
    return IpcRet::SUCCESS;
}

} //namespace end
