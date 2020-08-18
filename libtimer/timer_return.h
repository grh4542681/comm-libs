#ifndef __TIMER_RETURN_H__
#define __TIMER_RETURN_H__

#include "return.h"

namespace infra::timer {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        TIMER_EMODULE = TIMER_ERROR_CODE_MODULE,
        TIMER_EGET,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::TIMER_EMODULE)) {
            _exception.Push(ErrCode::TIMER_EMODULE, {
                { ErrCode::TIMER_EGET, "get time fail" }
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
