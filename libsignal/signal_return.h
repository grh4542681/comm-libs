#ifndef __SIGNAL_RETURN_H__
#define __SIGNAL_RETURN_H__

#include "return.h"

namespace infra::signal {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        SIGNAL_EMODULE = SIGNAL_ERROR_CODE_MODULE,

    //process signal
        SIGNAL_EINVAL,
        SIGNAL_EREGISTER,
        SIGNAL_ENOTFOUND,
        SIGNAL_EMASK,
        SIGNAL_EMASKTYPE,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::SIGNAL_EMODULE)) {
            _exception.Push(ErrCode::SIGNAL_EMODULE, {
                { ErrCode::SIGNAL_EINVAL, "Invalid signal" }
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
