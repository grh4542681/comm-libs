#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

#include "mempool.h"

#include "ipc_return.h"
#include "ipc_log.h"
#include "sem_sysv.h"


namespace infra::infra::ipc::sem {

SemSysV::SemSysV() : Sem()
{
    key_ = 0;
    semid_ = -1;
}

SemSysV::SemSysV(std::string path) : Sem(path)
{
    key_ = ftok(path.c_str(), 100);
    semid_ = -1;
}

/**
* @brief ~SemSysV - This function is destructor.
*/
SemSysV::~SemSysV(){
    if (init_flag_) {
        Close();
    }
}

bool SemSysV::IsExist()
{
    if (init_flag_) {
        return true;
    } else {
        if (semget(key_, 0, 0444) < 0) {
            int tmp_errno = errno;
            if (tmp_errno == ENOENT) {
                return false;
            } else {
                IPC_ERROR("%s", strerror(tmp_errno));
                return false;
            }
        } else {
            return true;
        }
    }
}

IpcRet SemSysV::Create(size_t semnum, mode_t mode)
{
    if (path_.empty() || semnum <= 0) {
        return IpcRet::EINIT;
    }

    if (semid_ > 0 || init_flag_) {
        return IpcRet::SUCCESS;
    }

    int tmp_errno;
    union semun args;

    semid_ = semget(key_, semnum, mode|IPC_CREAT|IPC_EXCL);
    if (semid_ < 0) {
        tmp_errno = errno;
        IPC_ERROR("%s", strerror(tmp_errno));
        return tmp_errno;
    }

    semnum_ = semnum;

    unsigned short* sem_val = (unsigned short*)(mempool::Mempool::getInstance()->Malloc(semnum * (sizeof(unsigned short))));
    memset(sem_val, 0, semnum* (sizeof(unsigned short)));

    args.array = sem_val;

    if (semctl(semid_, 0, SETALL, args) < 0) {
        tmp_errno = errno;
        mempool::Mempool::getInstance()->Free(sem_val);
        return tmp_errno;
    }
    mempool::Mempool::getInstance()->Free(sem_val);
    init_flag_ = true;
    return IpcRet::SUCCESS;
}

IpcRet SemSysV::Destroy()
{
    int tmp_errno;
    if (semid_ < 0) {
        semid_ = semget(key_, 0, 0);
        if (semid_ < 0) {
            tmp_errno = errno;
            IPC_ERROR("%s", strerror(tmp_errno));
            return tmp_errno;
        }
    }
    if (semctl(this->semid_, 0, IPC_RMID) < 0) {
        tmp_errno = errno;
        IPC_ERROR("%s", strerror(tmp_errno));
        return tmp_errno;
    }
    semid_ = -1;
    init_flag_ = false;
    return IpcRet::SUCCESS;
}

IpcRet SemSysV::Open(IpcMode mode)
{
    if (path_.empty()) {
        return IpcRet::EINIT;
    }

    if (semid_ > 0 || init_flag_) {
        return IpcRet::SUCCESS;
    }

    mode_t smode = 0;
    if (mode | IpcMode::READ_ONLY) {
        smode |= 0x04;
    } else if (mode | IpcMode::WRITE_ONLY) {
        smode |= 0x02;
    } else if (mode | IpcMode::READ_WRITE) {
        smode |= 0x06;
    } else {
        return IpcRet::SHM_EMODE;
    }

    semid_ = semget(key_, 0, smode);
    if (semid_ < 0) {
        int tmp_errno = errno;
        IPC_ERROR("%s", strerror(tmp_errno));
        return tmp_errno;
    }
    init_flag_ = true;
    return IpcRet::SUCCESS;
}

IpcRet SemSysV::Close()
{
    if (semid_ > 0 || init_flag_) {
        semid_ = -1;
        init_flag_ = false;
    }
    return IpcRet::SUCCESS;
}

IpcRet SemSysV::_p(size_t sem_index, timer::Time* overtime)
{
    if (semid_ <= 0 || sem_index > semnum_) {
        return IpcRet::EINIT;
    } 

    int tmperrno = 0;
    struct sembuf ops;
    memset(&ops, 0, sizeof(struct sembuf));

    ops.sem_num = sem_index;
    ops.sem_op = -1;
    ops.sem_flg |= SEM_UNDO;
    if (nonblock_flag_) {
        ops.sem_flg |= IPC_NOWAIT;
        if (semop(semid_, &ops, 1) < 0) {
            tmperrno = errno;
            IPC_ERROR("Semaphore set [%d] index [%ld] P operator failed, errno[%s]", semid_, sem_index, strerror(tmperrno));
            return tmperrno;
        }
    } else {
        ops.sem_flg &= ~IPC_NOWAIT;
        if (!overtime) {
            if (semop(semid_, &ops, 1) < 0) {
                tmperrno = errno;
                IPC_ERROR("Semaphore set [%d] index [%ld] P operator failed, errno[%s]", semid_, sem_index, strerror(tmperrno));
                return tmperrno;
            }
        } else {
            if (!overtime->IsPositive()) {
                IPC_ERROR("Overtime is not correct.");
                return IpcRet::EBADARGS;
            }
            timer::Time T_intervals = *overtime;

            struct timespec intervals;
            memset(&intervals, 0, sizeof(struct timespec));
            T_intervals.To(&intervals);

            timer::Time first_time = timer::Time::Now();

            do {
                if (semtimedop(semid_, &ops, 1, &intervals) < 0) {
                    tmperrno = errno;
                    if (tmperrno == EAGAIN) {
                        return IpcRet::ETIMEOUT;
                    } else if (tmperrno == EINTR) {
                        timer::Time second_time = timer::Time::Now();
                        T_intervals = T_intervals - (second_time - first_time);
                        IPC_LOG("Semaphore set [%d] index [%ld] P operator Interrupted by signal", semid_, sem_index);
                    } else {
                        IPC_ERROR("Semaphore set [%d] index [%ld] P operator failed, errno[%s]", semid_, sem_index, strerror(tmperrno));
                        return tmperrno;
                    }
                } else {
                    return IpcRet::SUCCESS;
                }
            } while (T_intervals.IsPositive());
        }
    }
    return IpcRet::SUCCESS;
}

IpcRet SemSysV::_v(size_t sem_index)
{
    int tmperrno = 0;

    struct sembuf ops;
    memset(&ops, 0, sizeof(struct sembuf));

    ops.sem_num = sem_index;
    ops.sem_op = 1;
    ops.sem_flg = 0;

    if (semop(semid_, &ops, 1) < 0) {
        tmperrno = errno;
        IPC_ERROR("Semaphore set [%d] index [%ld] V operator failed, errno[%s]", semid_, sem_index, strerror(tmperrno));
        return tmperrno;
    }
    return IpcRet::SUCCESS;
}

} //namespace infra::infra::ipc
