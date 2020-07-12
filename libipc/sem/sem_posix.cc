#include <unistd.h>
#include <stdio.h>
#include <string.h>


#include "ipc_log.h"
#include "sem_posix.h"

namespace ipc::sem {

SemPosix::SemPosix() : Sem()
{
    semset_.clear();
}

SemPosix::SemPosix(std::string path) : Sem(path)
{
    semset_.clear();
}

SemPosix::~SemPosix()
{
    if (init_flag_) {
        Close();
    }
}

bool SemPosix::IsExist()
{
    if (init_flag_) {
        return true;
    } else {
        std::string real_name  = path_ + "_1";
        if (access(real_name.c_str(), F_OK) < 0) {
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

IpcRet SemPosix::Create(size_t semnum, mode_t mode)
{
    if (path_.empty() || semnum <= 0 ) {
        return IpcRet::EINIT;
    }

    if (init_flag_) {
        return IpcRet::SUCCESS;
    }

    size_t loop = 0;
    for (loop = 0; loop < semnum; loop++) {
        std::string real_name  = path_ + "_" + std::to_string(loop);
        sem_t* psem = sem_open(real_name.c_str(), O_RDWR|O_CREAT|O_EXCL, mode, 0);
        if (psem == SEM_FAILED) {
            int tmperrno = errno;
            IPC_ERROR("Create posix semaphore set [%s] index[%ld] failed, errno[%s]", path_.c_str(), loop, strerror(tmperrno));
            return tmperrno;
        }
        semset_.insert_or_assign(real_name, psem);
    }
    semnum_ = semset_.size();
    init_flag_ = true;
    return IpcRet::SUCCESS;
}

IpcRet SemPosix::Destroy()
{
    Close();
    int semnum = 0;
    while (1) {
        std::string real_name = path_ + "_" + std::to_string(semnum);
        if (sem_unlink(real_name.c_str()) < 0) {
            int tmperrno = errno;
            if (tmperrno == ENOENT) {
                break;
            } else {
                IPC_ERROR("Destroy posix semaphore set [%s] realpath[%s] failed, errno[%s]", path_.c_str(), real_name.c_str(), strerror(tmperrno));
                return tmperrno;
            }
        }
        semnum++;
    }
    init_flag_ = false;
    return IpcRet::SUCCESS;
}

IpcRet SemPosix::Open(IpcMode mode)
{
    if (path_.empty()) {
        return IpcRet::EINIT;
    }

    if (init_flag_) {
        return IpcRet::SUCCESS;
    }

    int flags = 0;
    if (mode | IpcMode::READ_ONLY) {
        flags |= O_RDONLY;
    } else if (mode | IpcMode::WRITE_ONLY) {
        flags |= O_WRONLY;   
    } else if (mode | IpcMode::READ_WRITE) {
        flags |= O_RDWR;
    } else {
        return IpcRet::SEM_EMODE;
    }

    int semindex = 0;
    while(1) {
        std::string real_name = path_ + "_" + std::to_string(semindex);
        sem_t* psem = sem_open(real_name.c_str(), flags);
        if (psem == SEM_FAILED) {
            int tmperrno = errno;
            if (semindex == 0) {
                return tmperrno;
            } else {
                break;
            }
        }
        semset_.insert_or_assign(real_name, psem);
        semindex++;
    }
    semnum_ = semindex;
    init_flag_ = true;
    return IpcRet::SUCCESS;
}

IpcRet SemPosix::Close()
{
    if (!init_flag_) {
        return IpcRet::EINIT;
    }

    for(auto it : semset_) {
        sem_close(it.second);
    }
    semset_.clear();
    init_flag_ = false;

    return IpcRet::SUCCESS;
}

IpcRet SemPosix::_p(size_t sem_index, timer::Time* overtime)
{
    if (!init_flag_) {
        return IpcRet::EINIT;
    }
    if (sem_index >= semnum_ ) {
        return IpcRet::EBADARGS;
    }
    int tmperrno = 0;

    std::string real_name = path_ + "_" + std::to_string(sem_index);
    auto it = semset_.find(real_name);
    if (it == semset_.end()) {
        IPC_ERROR("Index [%ld] in semaphore set [%s] not found", sem_index, path_.c_str());
        return IpcRet::SEM_ENOTFOUND;
    }
    if (nonblock_flag_) {
        if (sem_trywait(it->second) < 0) {
            tmperrno = errno;
            IPC_ERROR("Semaphore set [%s] index [%ld] P operator failed, errno[%s]", path_.c_str(), sem_index, strerror(tmperrno));
            return tmperrno;
        }
    } else {
        if (!overtime) {
            if (sem_wait(it->second) < 0) {
                tmperrno = errno;
                IPC_ERROR("Semaphore set [%s] index [%ld] P operator failed, errno[%s]", path_.c_str(), sem_index, strerror(tmperrno));
                return tmperrno;
            }
        } else {
            if (!overtime->IsPositive()) {
                IPC_ERROR("Overtime is not correct.");
                return IpcRet::EBADARGS;
            }   

            timer::Time curr_time = timer::Time::Now();
            timer::Time T_intervals = curr_time + *overtime;

            struct timespec intervals;
            memset(&intervals, 0, sizeof(struct timespec));
            T_intervals.To(&intervals);

            while (curr_time < T_intervals) {
                if (sem_timedwait(it->second, &intervals) < 0) {
                    tmperrno = errno;
                    if (tmperrno == ETIMEDOUT) {
                        return IpcRet::ETIMEOUT;
                    } else if (tmperrno == EINTR) {
                        curr_time = timer::Time::Now();
                    } else {
                        IPC_ERROR("Semaphore set [%s] index [%ld] P operator failed, errno[%s]", path_.c_str(), sem_index, strerror(tmperrno));
                        return tmperrno;
                    }
                } else {
                    curr_time = timer::Time::Now();
                    *overtime = T_intervals - curr_time;
                    return IpcRet::SUCCESS;
                }
            }
        }
    }

    return IpcRet::SUCCESS;
}

IpcRet SemPosix::_v(size_t sem_index)
{
    if (!init_flag_) {
        return IpcRet::EINIT;
    }
    if (sem_index >= semnum_ ) {
        return IpcRet::EBADARGS;
    }

    int tmperrno = 0;

    std::string real_name = path_ + "_" + std::to_string(sem_index);
    auto it = semset_.find(real_name);
    if (it == semset_.end()) {
        IPC_ERROR("Index [%ld] in semaphore set [%s] not found", sem_index, path_.c_str());
        return IpcRet::SEM_ENOTFOUND;
    }

    if (sem_post(it->second) < 0) {
        tmperrno = errno;
        IPC_ERROR("Semaphore set [%s] index [%ld] P operator failed, errno[%s]", path_.c_str(), sem_index, strerror(tmperrno));
        return tmperrno;
    }
    return IpcRet::SUCCESS;
}

}
