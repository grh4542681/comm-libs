#ifndef __TIMER_RETURN_H__
#define __TIMER_RETURN_H__

#include "return.h"

namespace timer {

class Return : public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        TIMER_EMODULE = TIMER_ERROR_CODE_MODULE,
        TIMER_EGET,
    };
public:
    Return(int ecode) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::TIMER_EMODULE)) {
            _exception.Push(ErrCode::TIMER_EMODULE, {
                { ErrCode::TIMER_EGET, "get time fail" }
            });
        }
    }
    Return(Return& other) : base::Return(other) { }
    ~Return() { };

    Return& operator=(const int ecode) {
        Return::operator=(ecode);
        return *this;
    }   
    Return& operator=(const Return& ret) {
        Return::operator=(ret);
        return *this;
    }   
    Return& operator=(const Return&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
