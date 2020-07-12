#ifndef __IO_RETURN_H__
#define __IO_RETURN_H__

#include "return.h"

namespace io {

/**
* @brief - IO return value.
*/
class Return : public base::Return {
public:
    /**
    * @brief - Io return value.
    */
    enum ErrCode{
        IO_EMODULE = IO_ERROR_CODE_MODULE,
        IO_EFDTYPE,
        IO_EUNKNOWFD,
        IO_EUNKNOWEVENT,
        IO_EMATCHEVENT,
        IO_EADDEVENT,
        IO_ENOCALLBACK,

        IO_EBADSELECTITEM,
        IO_ENOSELECTITEM,

        IO_EINPUTCB,
        IO_EOUTPUTCB,
        IO_EERRCB,
    };
public:
    Return(int ecode) : base::Return(ecode) {
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
