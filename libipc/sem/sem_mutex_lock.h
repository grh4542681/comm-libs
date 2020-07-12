#ifndef __SEM_MUTEX_LOCK_H__
#define __SEM_MUTEX_LOCK_H__

#include <string>

#include "mempool.h"
#include "sem.h"
#include "sem_sysv.h"
#include "sem_posix.h"

namespace ipc::sem {

template < typename T = SemSysV >
class SemMutexLock {
public:
    SemMutexLock() {
        path_.clear();
        mp_ = mempool::Mempool::getInstance();
        sem_ = NULL;
        init_flag_ = false;
    }

    SemMutexLock(std::string path) {
        path_ = path;
        mp_ = mempool::Mempool::getInstance();;
        sem_ = NULL;
        init_flag_ = false;
    }

    ~SemMutexLock() {
        if (init_flag_) {
            Close();
        }
        if (sem_) {
            mp_->Free<T>(sem_);
        }
    }

    IpcRet Create() {
        return Create(0666);        
    }
    IpcRet Create(mode_t mode) {
        if (path_.empty) {
            return IpcRet::EINIT;
        }
        if (!sem_) {
            sem_ = mp_->Malloc<T>(path_);
        }
        IpcRet ret = sem_->Create(1, mode);
        if (ret == IpcRet::SUCCESS) {
            init_flag_ = true;
        } else {
            return ret;
        }
    }

    IpcRet Destroy() {
        if (path_.empty) {
            return IpcRet::EINIT;
        }
        if (!sem_) {
            sem_ = mp_->Malloc<T>(path_);
        }
        return sem_->Destroy();
    }

    IpcRet Open() {
        return Open(IpcMode::READ_WRITE);
    }
    IpcRet Open(IpcMode mode) {
        if (path_.empty) {
            return IpcRet::EINIT;
        }
        if (!sem_) {
            sem_ = mp_->Malloc<T>(path_);
        }
        IpcRet ret = sem_->Open(mode);
        if (ret == IpcRet::SUCCESS) {
            init_flag_ = true;
        } else {
            return ret;
        }
    }

    IpcRet Close() {
        if (!init_flag_) {
            return IpcRet::EINIT;
        }
        IpcRet ret = sem_->Close();
        if (ret == IpcRet::SUCCESS) {
            init_flag_ = false;
        } else {
            return ret;
        }
    }

    bool SetNonBlock(bool flag) {
        return sem_->SetNonBlock(flag);
    }

    IpcRet Lock(timer::Time* overtime) {
        if (!init_flag_) {
            return IpcRet::EINIT;
        }
        return sem_->P(0, overtime);
    }
    IpcRet UnLock() {
        if (!init_flag_) {
            return IpcRet::EINIT;
        }
        return sem_->V(0);
    }

private:
    std::string path_;
    Sem* sem_;
    mempool::Mempool* mp_;
    bool init_flag_;
};

}

#endif
