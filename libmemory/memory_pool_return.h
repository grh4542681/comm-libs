#ifndef __MEMORY_RETURN_H__
#define __MEMORY_RETURN_H__

#include "return.h"

namespace infra::memory {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        MEMORY_EMODULE = MEMORY_ERROR_CODE_MODULE,

    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::MEMORY_EMODULE)) {
            _exception.Push(ErrCode::MEMORY_EMODULE, {
                { ErrCode::MEMORY_EACCES, "No access" }
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
