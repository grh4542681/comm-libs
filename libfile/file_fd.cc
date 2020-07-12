#include "file_log.h"
#include "file_fd.h"
#include "file_api.h"

namespace file {

FileFD::FileFD() : io::FD()
{
    ffd_ = NULL;
}

FileFD::FileFD(unsigned int fd, bool auto_close) : io::FD(fd, auto_close)
{
    ffd_ = fdopen(fd, "w+");
    if (GetFileName(fd_, file_name_) != Return::SUCCESS) {
        file_name_.erase();
        fd_ = -1; 
        ffd_ = NULL;
        init_flag_ = false;
        FILE_ERROR("Get filename from file descriptor error.");
    }
}

FileFD::FileFD(FILE* ffd, bool auto_close)
{
    if (!ffd || fileno(ffd) < 0) {
        file_name_.erase();
        fd_ = -1; 
        ffd_ = NULL;
        init_flag_ = false;
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
            init_flag_ = true;
        }   
    }
}

FileFD::FileFD(FileFD& other) : io::FD(other)
{
    ffd_ = other.ffd_;
    file_name_ = other.file_name_;
}

FileFD::~FileFD()
{
    if (init_flag_ && auto_close_) {
        fclose(ffd_);
        close(fd_);
    }
}

base::Return FileFD::SetFD(unsigned int fd, bool auto_close)
{
    if (fd_ > 0 && init_flag_ && auto_close_) {
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
        init_flag_ = false;
        return temp_errno;
    }   
    if (GetFileName(fd_, file_name_) != Return::SUCCESS) {
        file_name_.erase();
        init_flag_ = false;
        FILE_ERROR("Get filename from file descriptor error.");
        return Return::ERROR;
    }
    ffd_ = fdopen(fd, "w+");
    fd_ = fd; 
    init_flag_ = true;
    auto_close_ = auto_close;
    return Return::SUCCESS;
}

base::Return FileFD::Dup(io::FD& new_fd)
{
    return Return::SUCCESS;
}

io::FD* FileFD::Clone()
{
    return alloc_.Allocate<FileFD>(*this);
}

void FileFD::Close()
{
    fclose(ffd_);
    close(fd_);
}

ssize_t FileFD::Write(const void* data, size_t datalen)
{
    if (!init_flag_) {
        return 0;
    }
    return fwrite(data, datalen, 1, ffd_);
}

ssize_t FileFD::Read(void* data, size_t datalen)
{
    if (!init_flag_) {
        return 0;
    }
    return fread(data, datalen, 1, ffd_);
}

std::string FileFD::GetName() const
{
    return file_name_;
}

Return FileFD::SetFD(FILE* ffd, bool auto_close)
{
    if (fd_ > 0 && init_flag_ && auto_close_) {
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
        init_flag_ = false;
        return temp_errno;
    }   
    if (GetFileName(fd_, file_name_) != Return::SUCCESS) {
        file_name_.erase();
        init_flag_ = false;
        FILE_ERROR("Get filename from file descriptor error.");
        return Return::ERROR;
    }
    ffd_ = ffd;
    init_flag_ = true;
    auto_close_ = auto_close;
    return Return::SUCCESS;
}


}
