#ifndef __THREAD_RETURN_H__
#define __THREAD_RETURN_H__

#include "return.h"

namespace infra::thread {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        THREAD_EMODULE = THREAD_ERROR_CODE_MODULE,

        THREAD_EREGISTER,
        THREAD_EUNREGISTER,
        THREAD_ERUNNING,
        THREAD_ESIGNALMASKTYPE,
        THREAD_ESIGNALMASK,

        THREAD_TEMPLATE_ECOUNTLIMIT,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::THREAD_EMODULE)) {
            _exception.Push(ErrCode::THREAD_EMODULE, {
                { ErrCode::THREAD_EREGISTER, "Register thread error" },
                { ErrCode::THREAD_TEMPLATE_ECOUNTLIMIT, "The number of thread template creations reaches the upper limit" },
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
