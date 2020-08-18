#ifndef __FILE_RETURN_H__
#define __FILE_RETURN_H__

#include "object.h"
#include "return.h"

namespace infra::file {

class Return : virtual public base::Object, public base::Return {
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
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::FILE_EMODULE)) {
            _exception.Push(ErrCode::FILE_EMODULE, {
                { ErrCode::FILE_ESTATE, "File state error" },
                { ErrCode::FILE_EACCES, "Can not access file" },
            });
        }
    }
    Return(Return& other) : base::Return(other) { }
    ~Return() { };

    Return& operator=(const int ecode) {
        base::Return::operator=(ecode);
        return *this;
    }   
    Return& operator=(const Return& ret) {
        base::Return::operator=(ret);
        return *this;
    }   
    Return& operator=(const Return&& ret) {
        base::Return::operator=(ret);
        return *this;
    }
};

}

#endif
