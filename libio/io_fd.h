#ifndef __IO_FD_H__
#define __IO_FD_H__

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "object.h"
#include "io_return.h"

namespace infra::io {

/**
* @brief - Base file descriptor class
*/
class FD : virtual public base::Object {
public:
    /**
    * @brief FD - Default consturctor
    */
    FD() {
        fd_ = -1;
        auto_close_ = false;
    }
    /**
    * @brief FD - Consturctor
    *
    * @param [fd] - Linux file descriptor
    * @param [auto_close] - Auto close flag
    */
    FD(unsigned int fd, bool auto_close = false) : fd_(fd), auto_close_(auto_close) {
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
    FD(FD& other) {
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
    FD(FD&& other) {
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
    FD(const FD& other) {
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
    FD(const FD&& other) {
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

    /**
    * @brief Valid - Whether the current file descriptor is valid.
    *
    * @returns  bool
    */
    bool Valid() {
        struct stat fd_stat;
        if (fstat(fd_, &fd_stat)) {
            return false;
        }
        return true;
    }

    /**
    * @brief SetNonBlock - Set the file descriptor to non-blocking mode
    *
    * @returns  Return
    */
    Return SetNonBlock() {
        if (Valid()) {
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

    /**
    * @brief SetBlock - Set the file descriptor to blocking mode
    *
    * @returns  Return
    */
    Return SetBlock() {
        if (Valid()) {
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

    /**
    * @brief GetAutoClose - Get the file descriptor auto-close option.
    *
    * @returns  
    */
    bool GetAutoClose() {
        return auto_close_;
    }

    /**
    * @brief SetAutoClose - Set the file descriptor auto-close option.
    *
    * @param [flag] - bool
    *
    * @returns  Self-reference
    */
    FD& SetAutoClose(bool flag) {
        auto_close_ = flag;
        return *this;
    }

    /**
    * @brief GetFD - Get real linux file descriptor.
    *
    * @returns  Real file descriptor.
    */
    int GetFD() const {
            return fd_;
    }

    /**
    * @brief SetFD - Set real linux file descriptor.
    *
    * @param [fd] - Real linux file descriptor.
    * @param [auto_close] - Auto-off option.
    *
    * @returns  Return
    */
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

    /**
    * @brief Dup - Duplicate a new file descriptor.
    *
    * @param [new_fd] - New file descriptor reference.
    *
    * @returns  Return
    */
    virtual Return Dup(FD& new_fd) {
        int fd = dup(fd_);
        if (fd < 0) {
            return errno;
        } else {
            new_fd.SetFD(fd, auto_close_);
            return Return::SUCCESS;
        }
    }

    /**
    * @brief Close - Close file descriptor.
    */
    virtual void Close() {
        close(fd_);
    }

    virtual FD* Clone() {
        return nullptr;
    }
    /**
    * @brief Write - Write input to file descriptor.
    *
    * @param [data] - Input data pointer.
    * @param [datalen] - Input data length.
    *
    * @returns  Write size.
    */
    virtual ssize_t Write(const void* data, size_t datalen) {
        return 0;
    }
    /**
    * @brief Read - Read output from file descriptor.
    *
    * @param [data] - Output data buffer.
    * @param [datalen] - Output data buffer length.
    *
    * @returns  Read size.
    */
    virtual ssize_t Read(void* data, size_t datalen) {
        return 0;
    }
protected:
    int fd_ = 0;
    bool auto_close_ = false;
};

}

#endif
