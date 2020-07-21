#ifndef __IO_FD_H__
#define __IO_FD_H__

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "allocator.h"
#include "io_return.h"

namespace infra::io {

/**
* @brief - Base file descriptor class
*/
class FD {
public:
    /**
    * @brief FD - Default consturctor
    */
    FD(base::Allocator&& alloc = base::Allocator()) : alloc_(alloc) {
        fd_ = -1;
        auto_close_ = false;
    }
    /**
    * @brief FD - Consturctor
    *
    * @param [fd] - Linux file descriptor
    * @param [auto_close] - Auto close flag
    */
    FD(unsigned int fd, bool auto_close = false, base::Allocator&& alloc = base::Allocator()) : fd_(fd), auto_close_(auto_close), alloc_(alloc){
        struct stat fd_stat;
        if (fstat(fd, &fd_stat)) {
            fd_ = -1;
        }
        auto_close_ = false;
    }
    /**
    * @brief FD - Copy constructor
    *
    * @param [other] -  Other instance
    */
    FD(FD& other) : alloc_(other.alloc_) {
        if (fd_ > 0 && auto_close_) {
            Close();
            fd_ = 0;
        }
        fd_ = other.fd_;
        auto_close_ = other.auto_close_;
    }
    /**
    * @brief FD - Copy constructor
    *
    * @param [other] -  Other instance
    */
    FD(FD&& other) : alloc_(other.alloc_) {
        if (fd_ > 0 && auto_close_) {
            Close();
            fd_ = 0;
        }
        fd_ = other.fd_;
        auto_close_ = other.auto_close_;
    }
    /**
    * @brief FD - Copy constructor
    *
    * @param [other] -  Other instance
    */
    FD(const FD& other) : alloc_(other.alloc_) {
        if (fd_ > 0 && auto_close_) {
            Close();
            fd_ = 0;
        }
        fd_ = other.fd_;
        auto_close_ = other.auto_close_;
    }
    /**
    * @brief FD - Copy constructor
    *
    * @param [other] -  Other instance
    */
    FD(const FD&& other) : alloc_(other.alloc_) {
        if (fd_ > 0 && auto_close_) {
            Close();
            fd_ = 0;
        }
        fd_ = other.fd_;
        auto_close_ = other.auto_close_;
    }
    /**
    * @brief ~FD - Destructor
    */
    virtual ~FD() {
        if (auto_close_) {
            Close();
        }
    }

    const FD& operator=(const FD& other) {
        if (fd_ > 0 && auto_close_) {
            Close();
            fd_ = 0;
        }
        fd_ = other.fd_;
        auto_close_ = other.auto_close_;
        return *this;
    }

    const FD& operator=(const FD&& other) {
        if (fd_ > 0 && auto_close_) {
            Close();
            fd_ = 0;
        }
        fd_ = other.fd_;
        auto_close_ = other.auto_close_;
        return *this;
    }

    bool operator==(const FD& other) const {
        return (fd_ == other.fd_);
    }
    bool operator==(const int& fd) const {
        return (fd_ == fd);
    }
    bool operator<(const FD& other) const {
        return (fd_ < other.fd_);
    }

    bool Available() {
        struct stat fd_stat;
        if (fstat(fd_, &fd_stat)) {
            return false;
        }
        return true;
    }

    Return SetNonBlock() {
        if (Available()) {
            int flags;
            if((flags = fcntl(fd_, F_GETFL, 0)) < 0)
            {
                return errno;
            }
            flags |= O_NONBLOCK;
            if(fcntl(fd_, F_SETFL, flags) < 0)
            {
                return errno;
            }
            return Return::SUCCESS;
        }
        return Return::IO_EAVALIABLE;
    }

    Return SetBlock() {
        if (Available()) {
            int flags;
            if((flags = fcntl(fd_, F_GETFL, 0)) < 0)
            {
                return errno;
            }
            flags &= ~O_NONBLOCK;
            if(fcntl(fd_, F_SETFL, flags) < 0)
            {
                return errno;
            }
            return Return::SUCCESS;
        }
    }

    bool GetAutoClose() {
        return auto_close_;
    }

    FD& SetAutoClose(bool flag) {
        auto_close_ = flag;
        return *this;
    }

    int GetFD() const {
            return fd_;
    }

    virtual Return SetFD(unsigned int fd, bool auto_close) {
        if (fd_ > 0) {
            Close();
            fd_ = -1;
        }
        struct stat fd_stat;
        if (fstat(fd, &fd_stat)) {
            fd_ = -1;
            auto_close_ = auto_close;
            return errno;
        }
        fd_ = fd;
        auto_close_ = auto_close;
        return Return::SUCCESS;
    }

    virtual Return Dup(FD& new_fd) {
        int fd = dup(fd_);
        if (fd < 0) {
            return errno;
        } else {
            new_fd.SetFD(fd, auto_close_);
            return Return::SUCCESS;
        }
    }

    virtual void Close() {
        close(fd_);
    }

    virtual FD* Clone() {
        return alloc_.Allocate<FD>(*this);
    }
    virtual ssize_t Write(const void* data, size_t datalen) {
        return 0;
    }
    virtual ssize_t Read(void* data, size_t datalen) {
        return 0;
    }
protected:
    int fd_ = 0;
    bool auto_close_ = false;
    base::Allocator& alloc_;
};

}

#endif
