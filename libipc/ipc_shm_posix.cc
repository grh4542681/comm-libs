/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-22 13:57
 * @file     : shm_posix.cc
 * @brief    : Posix share memory.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "shm_posix.h"

namespace infra::ipc::shm {

ShmPosix::ShmPosix(std::string path) : Shm(path)
{
    fd_ = -1;
}

ShmPosix::ShmPosix() : Shm()
{
    fd_ = -1;
}

ShmPosix::ShmPosix(const ShmPosix& other) : Shm()
{
    fd_ = other.fd_;
}

ShmPosix::~ShmPosix()
{
    if (fd_ > 0) {
        Close();
    }
}

IpcRet ShmPosix::Create(size_t size, mode_t mode)
{
    if (head_) {
        return IpcRet::SHM_ECREATED;
    }
    if (path_.empty() || size <= 0) {
        return IpcRet::EINIT;
    }
    fd_ = shm_open(path_.c_str(), O_RDWR|O_CREAT|O_EXCL, mode);
    if (fd_ < 0) {
        int tmp_errno = errno;
        IPC_ERROR("Create posix share memory error [%s]", strerror(tmp_errno));
        return tmp_errno;
    }
    if (ftruncate(fd_, size) < 0) {
        int tmp_errno = errno;
        close(fd_);
        fd_ = -1;
        return tmp_errno;
    }

    struct stat mstat;
    memset(&mstat, 0, sizeof(struct stat));
    if (fstat(fd_, &mstat) < 0) {
        int tmp_errno = errno;
        close(fd_);
        fd_ = -1;
        return tmp_errno;
    }
    size_ = mstat.st_size;

    close(fd_);
    fd_ = -1;
    status_ = ShmStatus::CREATE;
    return IpcRet::SUCCESS;
}

IpcRet ShmPosix::Destroy()
{
    if (shm_unlink(path_.c_str()) < 0) {
        int tmp_errno = errno;
        return tmp_errno;
    }
    status_ = ShmStatus::DESTROY;
    return IpcRet::SUCCESS;
}

IpcRet ShmPosix::Open(IpcMode mode)
{
    if (path_.empty()) {
        return IpcRet::EINIT;
    }

    int flags = 0;
    if ((mode | IpcMode::READ_ONLY) || (mode | IpcMode::READ_WRITE)) {
        flags |= PROT_READ;
    }
    if ((mode | IpcMode::WRITE_ONLY) || (mode | IpcMode::READ_WRITE)) {
        flags |= PROT_WRITE;   
    }

    if (fd_ < 0) {
        fd_ = shm_open(path_.c_str(), O_RDWR, 0);
        if (fd_ < 0) {
            int tmp_errno = errno;
            IPC_ERROR("Open posix share memory file error [%s]", strerror(tmp_errno));
            return tmp_errno;
        }
    }

    struct stat mstat;
    memset(&mstat, 0, sizeof(struct stat));
    if (fstat(fd_, &mstat) < 0) {
        int tmp_errno = errno;
        close(fd_);
        fd_ = -1;
        return tmp_errno;
    }
    size_ = mstat.st_size;

    head_ = mmap(NULL, size_, flags, MAP_SHARED, fd_, 0);
    if (head_ == MAP_FAILED) {
        int tmp_errno = errno;
        IPC_ERROR("Mmap posix share memory file error [%s]", strerror(tmp_errno));
        head_ = NULL;
        close(fd_);
        fd_ = -1;
        return tmp_errno;
    }

    close(fd_);
    fd_ = -1;
    status_ = ShmStatus::OPEN;
    return IpcRet::SUCCESS;
}

IpcRet ShmPosix::Close()
{
    if (head_ == NULL) {
        return IpcRet::EINIT;
    }
    if (munmap(head_, size_) < 0 ) {
        int tmp_errno = errno;
        return tmp_errno;
    }
    head_ = NULL;
    status_ = ShmStatus::CLOSE;
    return IpcRet::SUCCESS;
}

IpcRet ShmPosix::Sync()
{
    if (head_ == NULL) {
        return IpcRet::EINIT;
    }
    if (msync(head_, size_, MS_SYNC) < 0) {
        int tmp_errno = errno;
        return tmp_errno;
    }
    return IpcRet::SUCCESS;
}

} //namespace infra::end
