#ifndef __TIME_RETURN_H__
#define __TIME_RETURN_H__

#include "return.h"

namespace infra::time {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        TIME_EMODULE = TIME_ERROR_CODE_MODULE,
        TIME_INVALID,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::TIME_EMODULE)) {
            _exception.Push(ErrCode::TIME_EMODULE, {
                { ErrCode::TIME_INVALID, "Invalid time" },
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
