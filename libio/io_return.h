#ifndef __IO_RETURN_H__
#define __IO_RETURN_H__

#include "object.h"
#include "return.h"

namespace infra::io {

/**
* @brief - IO return value.
*/
class Return : virtual public base::Object, public base::Return {
public:
    /**
    * @brief - Io return value.
    */
    enum ErrCode {
        EDEFAULT,
        IO_EMODULE = IO_ERROR_CODE_MODULE,
        IO_EFDTYPE,
        IO_EUNKNOWFD,
        IO_EUNKNOWEVENT,
        IO_EMATCHEVENT,
        IO_EADDEVENT,
        IO_ENOCALLBACK,
        IO_EAVALIABLE,

        IO_EBADSELECTITEM,
        IO_ENOSELECTITEM,

        IO_EINPUTCB,
        IO_EOUTPUTCB,
        IO_EERRCB,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::IO_EMODULE)) {
            _exception.Push(ErrCode::IO_EMODULE, {
                { ErrCode::IO_EFDTYPE, "Bad file descriptor type" },
                { ErrCode::IO_EUNKNOWEVENT, "Unknow select event" },
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
