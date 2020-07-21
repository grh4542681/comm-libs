#include "file_log.h"
#include "file_fd.h"
#include "file_api.h"

namespace infra::file {

FD::FD() : io::FD()
{
    ffd_ = NULL;
}

FD::FD(unsigned int fd, bool auto_close) : io::FD(fd, auto_close)
{
    ffd_ = fdopen(fd, "w+");
    if (GetFileName(fd_, file_name_) != Return::SUCCESS) {
        file_name_.erase();
        fd_ = -1; 
        ffd_ = NULL;
        FILE_ERROR("Get filename from file descriptor error.");
    }
}

FD::FD(FILE* ffd, bool auto_close)
{
    if (!ffd || fileno(ffd) < 0) {
        file_name_.erase();
        fd_ = -1; 
        ffd_ = NULL;
        FILE_ERROR("File stream pointer invalid.");
    } else {
        fd_ = fileno(ffd);
        if (GetFileName(fd_, file_name_) != Return::SUCCESS) {
            file_name_.erase();
            fd_ = -1; 
            ffd_ = NULL;
            FILE_ERROR("Get filename from file descriptor error.");
        } else {
            ffd_ = ffd;
            auto_close_ = auto_close;
        }   
    }
}

FD::FD(FD& other) : io::FD(other)
{
    ffd_ = other.ffd_;
    file_name_ = other.file_name_;
}

FD::~FD()
{
    if (auto_close_) {
        fclose(ffd_);
    }
}

io::Return FD::SetFD(unsigned int fd, bool auto_close)
{
    if (fd_ > 0 && auto_close_) {
        fclose(ffd_);
        close(fd_);
        ffd_ = NULL;
        fd_ = 0;
        file_name_.erase();
    }   
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd, &fd_stat)) {
        temp_errno = errno;
        FILE_ERROR("%s", strerror(temp_errno));
        return temp_errno;
    }   
    if (GetFileName(fd_, file_name_) != Return::SUCCESS) {
        file_name_.erase();
        FILE_ERROR("Get filename from file descriptor error.");
        return Return::ERROR;
    }
    ffd_ = fdopen(fd, "w+");
    fd_ = fd; 
    auto_close_ = auto_close;
    return Return::SUCCESS;
}

io::Return FD::Dup(FD& new_fd)
{
    return Return::SUCCESS;
}

FD* FD::Clone()
{
    return alloc_.Allocate<FD>(*this);
}

void FD::Close()
{
    fclose(ffd_);
    close(fd_);
}

ssize_t FD::Write(const void* data, size_t datalen)
{
    if (!Available()) {
        return 0;
    }
    return fwrite(data, datalen, 1, ffd_);
}

ssize_t FD::Read(void* data, size_t datalen)
{
    if (!Available()) {
        return 0;
    }
    return fread(data, datalen, 1, ffd_);
}

std::string FD::GetName() const
{
    return file_name_;
}

io::Return FD::SetFD(FILE* ffd, bool auto_close)
{
    if (fd_ > 0) {
        fclose(ffd_);
        close(fd_);
        ffd_ = NULL;
        fd_ = 0;
        file_name_.erase();
    }   
    fd_ = fileno(ffd);
    int temp_errno = 0;
    struct stat fd_stat;
    if (fstat(fd_, &fd_stat)) {
        temp_errno = errno;
        FILE_ERROR("%s", strerror(temp_errno));
        return temp_errno;
    }   
    if (GetFileName(fd_, file_name_) != Return::SUCCESS) {
        file_name_.erase();
        FILE_ERROR("Get filename from file descriptor error.");
        return Return::ERROR;
    }
    ffd_ = ffd;
    auto_close_ = auto_close;
    return Return::SUCCESS;
}


}
