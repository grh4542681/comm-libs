#ifndef __IO_RETURN_H__
#define __IO_RETURN_H__

#include "return.h"

namespace infra {

/**
* @brief - IO return value.
*/
class IoReturn : public Return {
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
    IoReturn(int ecode) : Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::IO_EMODULE)) {
            _exception.Push(ErrCode::IO_EMODULE, {
                { ErrCode::IO_EFDTYPE, "Bad file descriptor type" },
                { ErrCode::IO_EUNKNOWEVENT, "Unknow select event" },
            });
        }
    }
    IoReturn(IoReturn& other) : Return(other) { }
    ~IoReturn() { };

    IoReturn& operator=(const int ecode) {
        Return::operator=(ecode);
        return *this;
    }   
    IoReturn& operator=(const IoReturn& ret) {
        Return::operator=(ret);
        return *this;
    }   
    IoReturn& operator=(const IoReturn&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
