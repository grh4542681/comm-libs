#ifndef __SEM_RW_LOCK_H__
#define __SEM_RW_LOCK_H__

#include <string>

#include "mempool.h"
#include "timer_time.h"

#include "shm/shm.h"
#include "shm/shm_sysv.h"
#include "shm/shm_posix.h"
#include "sem.h"
#include "sem_sysv.h"
#include "sem_posix.h"

namespace ipc::sem {

enum class SemRWLockMode {
    PreferRead,
    PreferWrite,
};

enum class SemRWLockState {
    RLocked,
    WLocked,
    NLocked,
};

template < typename SEMT = SemSysV, typename SHMT = shm::ShmSysV>
class SemRWLock {
public:
    struct SemRWLockInfo {
        unsigned int user_num_;
        SemRWLockMode rwlock_mode_;
        unsigned int reader_num_;
        unsigned int wait_reader_num_;
        unsigned int writer_num_;
        unsigned int wait_writer_num_;
    };

public:
    SemRWLock(std::string path) {
        path_ = path;
        sem_path_ = path_ + "sem";
        shm_path_ = path_ + "shm";
        sem_ = NULL;
        shm_ = NULL;
        rwlock_info_ = NULL;
        mempool_ = mempool::Mempool::getInstance();
        init_flag_ = false;
        rwlock_state_ = SemRWLockState::NLocked;
    }
    ~SemRWLock() {
        if (init_flag_) {
            Close();
            init_flag_ = false;
        }
        if (sem_) {
            mempool_->Free<SEMT>(sem_);
        }
        if (shm_) {
            mempool_->Free<SHMT>(shm_);
        }
    }

    IpcRet Create(mode_t mode) {
        IpcRet ret = IpcRet::SUCCESS;
        sem_ = mempool_->Malloc<SEMT>(sem_path_);
        if (!sem_) {
            IPC_ERROR("Alloc memory for sem[%s] failed in RWLock", sem_path_.c_str());
            return IpcRet::EMALLOC;
        }
        ret = sem_->Create(3, mode);
        if (ret != IpcRet::SUCCESS) {
            IPC_ERROR("Create sem[%s] failed in RWLock, ret[%d][%s]", sem_path_.c_str(), ret.Code(), ret.Message().c_str());
            return ret;
        }

        shm_ = mempool_->Malloc<SHMT>(shm_path_);
        if (!shm_) {
            IPC_ERROR("Alloc memory for shm[%s] failed in RWLock", shm_path_.c_str());
            return IpcRet::EMALLOC;
        }
        ret = shm_->Create(sizeof(struct SemRWLockInfo)+200, mode);
        if (ret != IpcRet::SUCCESS) {
            IPC_ERROR("Create shm[%s] failed in RWLock, ret[%d][%s]", shm_path_.c_str(), ret.Code(), ret.Message().c_str());
            return ret;
        }
        shm_->Open(IpcMode::READ_WRITE);

        rwlock_info_ = reinterpret_cast<struct SemRWLockInfo*>(shm_->GetHeadPtr());
        rwlock_info_->user_num_ = 0;
        rwlock_info_->rwlock_mode_ = SemRWLockMode::PreferRead;
        rwlock_info_->reader_num_ = 0;
        rwlock_info_->wait_reader_num_ = 0;
        rwlock_info_->writer_num_ = 0;
        rwlock_info_->wait_writer_num_ = 0;

        shm_->Close();
        rwlock_info_ = NULL;
        _ctrl_mutex_unlock();

        return ret;
    }

    IpcRet Destroy() {
        IpcRet ret = IpcRet::SUCCESS;
        if (sem_) {
            ret =sem_->Destroy();
            sem_ = NULL;
        } else {
            SEMT sem(sem_path_);
            ret = sem.Destroy();
        }
        if (ret != IpcRet::SUCCESS) {
            IPC_ERROR("Destroy sem[%s] failed in RWLock, ret[%d][%s]", sem_path_.c_str(), ret.Code(), ret.Message().c_str());
            return ret;
        }

        if (shm_) {
            ret = shm_->Destroy();
            shm_ = NULL;
        } else {
            SHMT shm(shm_path_);
            ret = shm.Destroy();
        }
        if (ret != IpcRet::SUCCESS) {
            IPC_ERROR("Destroy shm[%s] failed in RWLock, ret[%d][%s]", shm_path_.c_str(), ret.Code(), ret.Message().c_str());
            return ret;
        }
        return ret;
    }

    IpcRet Open() {
        IpcRet ret = IpcRet::SUCCESS;
        if (!sem_) {
            sem_ = mempool_->Malloc<SEMT>(sem_path_);
            if (!sem_) {
                IPC_ERROR("Alloc memory for sem[%s] failed in RWLock", sem_path_.c_str());
                return IpcRet::EMALLOC;
            }
        }
        if (!shm_) {
            shm_ = mempool_->Malloc<SHMT>(shm_path_);
            if (!shm_) {
                IPC_ERROR("Alloc memory for shm[%s] failed in RWLock", shm_path_.c_str());
                return IpcRet::EMALLOC;
            }
        }
        ret = sem_->Open(IpcMode::READ_WRITE);
        if (ret != IpcRet::SUCCESS) {
            return ret;
        }
        ret = shm_->Open(IpcMode::READ_WRITE);
        if (ret != IpcRet::SUCCESS) {
            return ret;
        }
        rwlock_info_ = reinterpret_cast<struct SemRWLockInfo*>(shm_->GetHeadPtr());
        init_flag_ = true;
        return ret;
    }

    IpcRet Close() {
        IpcRet ret = IpcRet::SUCCESS;
        if (init_flag_) {
            UnLock(NULL);
            ret = sem_->Close();
            if (ret != IpcRet::SUCCESS) {
                return ret;
            }
            ret = shm_->Close();
            if (ret != IpcRet::SUCCESS) {
                return ret;
            }
        } else {
            ret = IpcRet::EINIT;
        }
        init_flag_ = false;
        return ret;
    }

    IpcRet SetPerfer(SemRWLockMode mode) {
        IpcRet ret = IpcRet::SUCCESS;
        ret = _ctrl_mutex_lock();
        if (ret == IpcRet::SUCCESS) {
            rwlock_info_->rw_mode_ = mode;
            _ctrl_mutex_unlock();
            return IpcRet::SUCCESS;
        } else {
            return ret;
        }
    }

    bool SetNonBlock(bool flag) {
        if (sem_) {
            return sem_->SetNonBlock(flag);
        }
        return false;
    }

    IpcRet RLock(timer::Time* overtime) {
        if (rwlock_state_ != SemRWLockState::NLocked) {
            return IpcRet::SEM_ELOCKED;
        }

        IpcRet ret = IpcRet::SUCCESS;

        if ((ret = _ctrl_mutex_lock(overtime)) != IpcRet::SUCCESS) {
            if (ret == IpcRet::ETIMEOUT) {
                IPC_WARN("RW lock[%s] ctrl mutex locked timeout.", path_.c_str());
            } else {
                IPC_ERROR("RW lock[%s] ctrl mutex locked failed.", path_.c_str());
            }
            return ret;
        }
        printf("---%d----\n", __LINE__);
        switch (rwlock_info_->rwlock_mode_) {
            case SemRWLockMode::PreferRead:
        printf("---%d----\n", __LINE__);
                if (rwlock_info_->writer_num_ == 0) {
                    rwlock_info_->reader_num_++;
                    _ctrl_mutex_unlock();
                    ret = IpcRet::SUCCESS;
        printf("---%d----\n", __LINE__);
                } else {
                    rwlock_info_->wait_reader_num_++;
                    _ctrl_mutex_unlock();
                    ret = _read_mutex_lock(overtime);
                }
                break;
            case SemRWLockMode::PreferWrite:
                if (rwlock_info_->writer_num_ == 0 && rwlock_info_->wait_writer_num_ == 0) {
                    rwlock_info_->reader_num_++;
                    _ctrl_mutex_unlock();
                    ret = IpcRet::SUCCESS;
                } else {
                    rwlock_info_->wait_reader_num_++;
                    _ctrl_mutex_unlock();
                    ret = _read_mutex_lock(overtime);
                }
                break;
            default:
                _ctrl_mutex_unlock();
                IPC_ERROR("Unknow SemRWLockMode[%d]", static_cast<int>(rwlock_info_->rwlock_mode_));
                break;
        }

        if (ret == IpcRet::SUCCESS) {
            rwlock_state_ = SemRWLockState::RLocked;
        }
        return ret;
    }

    IpcRet WLock(timer::Time* overtime) {
        if (rwlock_state_ != SemRWLockState::NLocked) {
            return IpcRet::SEM_ELOCKED;
        }

        IpcRet ret = IpcRet::SUCCESS;

        if ((ret = _ctrl_mutex_lock(overtime)) != IpcRet::SUCCESS) {
            if (ret == IpcRet::ETIMEOUT) {
                IPC_WARN("RW lock[%s] ctrl mutex locked timeout.", path_.c_str());
            } else {
                IPC_ERROR("RW lock[%s] ctrl mutex locked failed.", path_.c_str());
            }
            return ret;
        }
        switch (rwlock_info_->rwlock_mode_) {
            case SemRWLockMode::PreferRead:
                if (rwlock_info_->writer_num == 0 && rwlock_info_->reader_num_ == 0 && rwlock_info_->wait_reader_num_ == 0) {
                    rwlock_info_->writer_num_++;
                    _ctrl_mutex_unlock();
                    ret = IpcRet::SUCCESS;
                } else {
                    rwlock_info_->wait_writer_num_++;
                    _ctrl_mutex_unlock();
                    ret = _write_mutex_lock(overtime);
                }
                break;
            case SemRWLockMode::PreferWrite:
                if (rwlock_info_->writer_num == 0 && rwlock_info_->reader_num_ == 0) {
                    rwlock_info_->writer_num_++;
                    _ctrl_mutex_unlock();
                    ret = IpcRet::SUCCESS;
                } else {
                    rwlock_info_->wait_writer_num_++;
                    _ctrl_mutex_unlock();
                    ret = _write_mutex_lock(overtime);
                }
                break;
            default:
                _ctrl_mutex_unlock();
                ret = IpcRet::EBADARGS;
                IPC_ERROR("Unknow SemRWLockMode[%d]", rwlock_info_->rwlock_mode_);
                break;
        }

        if (ret == IpcRet::SUCCESS) {
            rwlock_state_ = SemRWLockState::WLocked;
        }
        return ret;
        
    }

    IpcRet UnLock(timer::Time* overtime) {
        IpcRet ret = IpcRet::SUCCESS;
        switch (rwlock_state_) {
            case SemRWLockState::NLocked:
                ret = IpcRet::SUCCESS;
                break;
            case SemRWLockState::RLocked:
                if ((ret = _ctrl_mutex_lock(overtime)) != IpcRet::SUCCESS) {
                    if (ret == IpcRet::ETIMEOUT) {
                        IPC_WARN("RW lock[%s] ctrl mutex locked timeout.", path_.c_str());
                    } else {
                        IPC_ERROR("RW lock[%s] ctrl mutex locked failed.", path_.c_str());
                    }
                    return ret;
                }
                switch (rwlock_info_->rwlock_mode_) {
                    case SemRWLockMode::PreferRead:
                        rwlock_info_->reader_num_--;
                        if (rwlock_info_->wait_reader_num_ > 0) {
                            auto loop = rwlock_info_->wait_reader_num_;
                            for (; loop > 0; loop--) {
                                _read_mutex_unlock();
                                rwlock_info_->wait_reader_num_--;
                                rwlock_info_->reader_num_++;
                            }
                        } else {
                            if (rwlock_info_->reader_num_ == 0 && rwlock_info_->wait_writer_num_ > 0) {
                                _write_mutex_unlock();
                                rwlock_info_->wait_writer_num_--;
                                rwlock_info_->writer_num_++;
                            }
                        }
                        ret = IpcRet::SUCCESS;
                        break;
                    case SemRWLockMode::PreferWrite:
                        rwlock_info_->reader_num_--;
                        if (rwlock_info_->reader_num_ == 0) {
                            if (rwlock_info_->wait_writer_num_ > 0) {
                                _write_mutex_unlock();
                                rwlock_info_->wait_writer_num_--;
                                rwlock_info_->writer_num_++;
                            } else {
                                if (rwlock_info_->wait_reader_num_ > 0) {
                                    auto loop = rwlock_info_->wait_reader_num_;
                                    for (; loop > 0; loop--) {
                                        _read_mutex_unlock();
                                        rwlock_info_->wait_reader_num_--;
                                        rwlock_info_->reader_num_++;
                                    }
                                }
                            }
                        }
                        ret = IpcRet::SUCCESS;
                        break;
                    default:
                        ret = IpcRet::EBADARGS;
                        IPC_ERROR("Unknow SemRWLockMode[%d]", (int)(rwlock_info_->rwlock_mode_));
                        break;
                }
                _ctrl_mutex_unlock();
                break;
            case SemRWLockState::WLocked:
                if ((ret = _ctrl_mutex_lock(overtime)) != IpcRet::SUCCESS) {
                    if (ret == IpcRet::ETIMEOUT) {
                        IPC_WARN("RW lock[%s] ctrl mutex locked timeout.", path_.c_str());
                    } else {
                        IPC_ERROR("RW lock[%s] ctrl mutex locked failed.", path_.c_str());
                    }
                    return ret;
                }
                switch (rwlock_info_->rwlock_mode_) {
                    case SemRWLockMode::PreferRead:
                        rwlock_info_->writer_num_--;
                        if (rwlock_info_->wait_reader_num_ > 0) {
                            auto loop = rwlock_info_->wait_reader_num_;
                            for (; loop > 0; loop--) {
                                _read_mutex_unlock();
                                rwlock_info_->wait_reader_num_--;
                                rwlock_info_->reader_num_++;
                            }
                        } else {
                            if (rwlock_info_->reader_num_ == 0 && rwlock_info_->wait_writer_num_ > 0) {
                                _write_mutex_unlock();
                                rwlock_info_->wait_writer_num_--;
                                rwlock_info_->writer_num_++;
                            }
                        }
                        ret = IpcRet::SUCCESS;
                        break;
                    case SemRWLockMode::PreferWrite:
                        rwlock_info_->writer_num_--;
                        if (rwlock_info_->writer_num_ == 0) {
                            if (rwlock_info_->wait_writer_num_ > 0) {
                                _write_mutex_unlock();
                                rwlock_info_->wait_writer_num_--;
                                rwlock_info_->writer_num_++;
                            } else {
                                if (rwlock_info_->wait_reader_num_ > 0) {
                                    auto loop = rwlock_info_->wait_reader_num_;
                                    for (; loop > 0; loop--) {
                                        _read_mutex_unlock();
                                        rwlock_info_->wait_reader_num_--;
                                        rwlock_info_->reader_num_++;
                                    }
                                }
                            }
                        }
                        ret = IpcRet::SUCCESS;
                        break;
                    default:
                        ret = IpcRet::EBADARGS;
                        IPC_ERROR("Unknow SemRWLockMode[%d]", (int)(rwlock_info_->rwlock_mode_));
                        break;
                }
                _ctrl_mutex_unlock();
                break;
            default:
                ret = IpcRet::EBADARGS;
                IPC_ERROR("Unknow SemRWLock current state[%d]", (int)(rwlock_state_));
                break;
        }
        rwlock_state_ = SemRWLockState::NLocked;
        return ret;
    }

    void print() {
        printf("---%d----\n", __LINE__);
        _ctrl_mutex_lock(NULL);
        printf("reader[%u]\n", rwlock_info_->reader_num_);
        _ctrl_mutex_unlock();
    }
private:
    mempool::Mempool* mempool_;
    bool init_flag_;
    std::string path_;

    std::string shm_path_;
    std::string sem_path_;
    SHMT* shm_;
    SEMT* sem_;
    struct SemRWLockInfo* rwlock_info_;
    SemRWLockState rwlock_state_;

    IpcRet _ctrl_mutex_lock(timer::Time* overtime) {
        return sem_->P(0, overtime);
    }
    IpcRet _ctrl_mutex_unlock() {
        return sem_->V(0);
    }

    IpcRet _read_mutex_lock(timer::Time* overtime) {
        return sem_->P(1, overtime);
    }
    IpcRet _read_mutex_unlock() {
        return sem_->V(1);
    }

    IpcRet _write_mutex_lock(timer::Time* overtime) {
        return sem_->P(2, overtime);
    }
    IpcRet _write_mutex_unlock() {
        return sem_->V(2);
    }
};

}

#endif
