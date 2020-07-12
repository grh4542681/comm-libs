#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <map>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "file_log.h"
#include "file_return.h"
#include "file_fd.h"

#define FILE_MAX_LINE_LEN (1024)

namespace file {

class File {
public:
    enum class Format : int {
        Unknow = 0,
        Ini,
        Xml,
        Json,
        Yaml,
        Txt,
    };

    enum class State {
        Invalid,
        Opened,
        Closed,
    };

    enum Mode {
        READ_ONLY = 0x01,
        WRITE_ONLY = 0x02,
        READ_WRITE = 0x01 | 0x02,
        CREAT = 0x08,
        EXIST = 0x10,
        TRUNC = 0x20,
        APPEND = 0x40, 
    };

    class Attrbute {
    private:
        Attrbute();
        Attrbute(Attrbute& other);
        ~Attrbute();

    private:
        struct stat stat_;
    };

public:
    static std::map<File::Format, std::vector<std::string>> SupportFormat;

public:
    File(std::string filename);
    File(const FileFD& fd_);
    File(const FileFD&& fd_);
    ~File();

    FileFD& GetFileFD();
    std::string GetFileName();
    Format GetFileFormat();
    std::string GetFileFormatDescribe();

    Return Open(int mode, bool auto_close = false);
    Return Open(Mode mode, bool auto_close = false);
    Return Close();

    static Return ModeConvert(int mode, std::string& smode);

private:
    std::string file_name_;
    bool auto_close_ = false;
    FileFD fd_;
    State state_;
    Format format_;

};

}//namespcae file end

#endif
