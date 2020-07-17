#include <string.h>
#include <unistd.h>

#include "file.h"
#include "file_api.h"

namespace infra {

std::map<File::Format, std::vector<std::string>> File::SupportFormat = {
    {File::Format::Ini, {"ini", "Initialization File"}},
    {File::Format::Xml, {"xml", "Extensible Markup Language"}},
    {File::Format::Json, {"json", "JavaScript Object Notation"}},
    {File::Format::Yaml, {"yaml", "YAML Ain\'t a Markup Language"}},
    {File::Format::Txt, {"txt", "Text file"}},
    {File::Format::Unknow, {"", "Unknow file format"}},
};

File::File(std::string filename)
{
    file_name_ = filename;
    format_ = Format::Unknow;
    std::string file_ext = GetFileExtension(file_name_);
    for (auto it : File::SupportFormat) {
        if (file_ext == it.second[0]) {
            format_ = it.first;
        }
    }
    state_ = State::Closed;
}

File::File(const FileFD& fd)
{
    format_ = Format::Unknow;
    if (!fd.Initalize()) {
        file_name_.erase();
        state_ = State::Invalid;
        FILE_ERROR("File descriptor invalid.");
    } else {
        file_name_ = fd.GetName();
        fd_ = fd;
        std::string file_ext = GetFileExtension(file_name_);
        for (auto it : File::SupportFormat) {
            if (file_ext == it.second[0]) {
                format_ = it.first;
            }
        }
        state_ = State::Opened;
    }
}

File::File(const FileFD&& fd)
{
    format_ = Format::Unknow;
    if (!fd.Initalize()) {
        file_name_.erase();
        state_ = State::Invalid;
        FILE_ERROR("File descriptor invalid.");
    } else {
        file_name_ = fd.GetName();
        fd_ = fd;
        std::string file_ext = GetFileExtension(file_name_);
        for (auto it : File::SupportFormat) {
            if (file_ext == it.second[0]) {
                format_ = it.first;
            }
        }
        state_ = State::Opened;
    }
}

File::~File()
{
    if (state_ == State::Opened && auto_close_) {
        Close();
    }
}

FileFD& File::GetFileFD()
{
    return fd_;
}

std::string File::GetFileName()
{
    return file_name_;
}

File::Format File::GetFileFormat()
{
    return format_;
}

std::string File::GetFileFormatDescribe()
{
    return File::SupportFormat.find(format_)->second[1];
}

FileReturn File::Open(Mode mode, bool auto_close)
{
    return Open((int)mode, auto_close);
}

FileReturn File::Open(int mode, bool auto_close)
{
    if (state_ == State::Invalid) {
        return FileReturn::EINIT;
    } else if (state_ == State::Opened) {
        return FileReturn::SUCCESS;
    } else if (state_ == State::Closed) {
        std::string smode;
        ModeConvert(mode, smode);
        FILE* ffd = fopen(file_name_.c_str(), smode.c_str());
        if (!ffd) {
            return FileReturn::ERROR;
        }
        fd_.SetFD(ffd, auto_close);
        state_ = State::Opened;
        return FileReturn::SUCCESS;        
    } else {
        return FileReturn::FILE_ESTATE;
    }
}

FileReturn File::Close()
{
    fd_.Close();
    state_ = State::Closed;
    return FileReturn::SUCCESS;
}

FileReturn File::ModeConvert(int mode, std::string& smode) {
    char cmode[16];
    memset(cmode, 0x00, sizeof(cmode));
    if (mode == Mode::READ_ONLY) {
        sprintf(cmode, "%s", "r");
    } else if (mode == Mode::WRITE_ONLY) {
        sprintf(cmode, "%s", "w");
    } else if (mode == Mode::READ_WRITE) {
        sprintf(cmode, "%s", "r+");
    } else if (mode == (Mode::WRITE_ONLY | Mode::CREAT)) {
        sprintf(cmode, "%s", "w+");
    } else if (mode == Mode::APPEND) {
        sprintf(cmode, "%s", "a+");
    } else {
        return FileReturn::FILE_EMODE;
    }   
    smode.assign(cmode);
    return FileReturn::SUCCESS;
}

}
