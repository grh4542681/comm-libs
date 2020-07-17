#ifndef __TIMER_RETURN_H__
#define __TIMER_RETURN_H__

#include "return.h"

namespace infra {

class TimerReturn : public Return {
public:
    enum ErrCode{
        EDEFAULE,
        TIMER_EMODULE = TIMER_ERROR_CODE_MODULE,
        TIMER_EGET,
    };
public:
    TimerReturn(int ecode) : Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::TIMER_EMODULE)) {
            _exception.Push(ErrCode::TIMER_EMODULE, {
                { ErrCode::TIMER_EGET, "get time fail" }
            });
        }
    }
    TimerReturn(TimerReturn& other) : Return(other) { }
    ~TimerReturn() { };

    TimerReturn& operator=(const int ecode) {
        Return::operator=(ecode);
        return *this;
    }   
    TimerReturn& operator=(const TimerReturn& ret) {
        Return::operator=(ret);
        return *this;
    }   
    TimerReturn& operator=(const TimerReturn&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
