#ifndef __FILE_FD_H__
#define __FILE_FD_H__

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "io_fd.h"

#include "file_return.h"

#define FILE_MAX_LINE_LEN1 1024

namespace infra::file {

class FD : public io::FD {
public:
    FD();
    FD(unsigned int fd, bool auto_close = false);
    FD(FILE* ffd, bool auto_close = false);
    FD(FD& other);
    ~FD();

    //Inherited from class io::FD.
    io::Return SetFD(unsigned int fd, bool auto_close);
    io::Return Dup(FD& new_fd);
    FD* Clone();
    void Close();
    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

    std::string GetName() const;

    io::Return SetFD(FILE* ffd, bool auto_close);
    template <typename ... Args>
    int ReadFmt(const char* fmt, Args&& ... args) {
        char line[FILE_MAX_LINE_LEN1];
        memset(line, 0x00, sizeof(line));
        int ret = Read(line, sizeof(line));
        if (ret <= 0) {
            return ret;
        }   
        sscanf(line, fmt, std::forward<Args>(args)...);
        return ret;
    }   

    template <typename ... Args>
    int WriteFmt(const char* fmt, Args&& ... args) {
        char line[FILE_MAX_LINE_LEN1];
        memset(line, 0x00, sizeof(line));
        sprintf(line, fmt, std::forward<Args>(args)...);
        return Write(line, sizeof(line));
    }

private:
    std::string file_name_;
    FILE* ffd_;
};

}

#endif
