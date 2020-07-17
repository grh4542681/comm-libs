#ifndef __FILE_RETURN_H__
#define __FILE_RETURN_H__

#include "return.h"

namespace infra {

class FileReturn : public Return {
public:
    enum ErrCode{
        EDEFAULT,
        FILE_EMODULE = FILE_ERROR_CODE_MODULE,
        FILE_ESTATE,
        FILE_EACCES,
        FILE_EFAULT,
        FILE_EINVAL,
        FILE_EIO,
        FILE_ELOOP,
        FILE_ENAMETOOLONG,
        FILE_ENOENT,
        FILE_ENOMEM,
        FILE_ENOTDIR,
        FILE_EMODE,
    };
public:
    FileReturn(int ecode) : Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::FILE_EMODULE)) {
            _exception.Push(ErrCode::FILE_EMODULE, {
                { ErrCode::FILE_ESTATE, "File state error" },
                { ErrCode::FILE_EACCES, "Can not access file" },
            });
        }
    }
    FileReturn(FileReturn& other) : Return(other) { }
    ~FileReturn() { };

    FileReturn& operator=(const int ecode) {
        Return::operator=(ecode);
        return *this;
    }   
    FileReturn& operator=(const FileReturn& ret) {
        Return::operator=(ret);
        return *this;
    }   
    FileReturn& operator=(const FileReturn&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
