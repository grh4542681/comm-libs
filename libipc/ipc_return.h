#ifndef __IPC_RETURN_H__
#define __IPC_RETURN_H__

#include "object.h"
#include "return.h"

namespace infra::ipc {

/**
* @brief - IPC return value.
*/
class Return : virtual public base::Object, public base::Return {
public:
    /**
    * @brief - Io return value.
    */
    enum ErrCode {
        IPC_EAVALIABLE,
        IPC_EMODULE = IPC_ERROR_CODE_MODULE,

    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::IPC_EMODULE)) {
            _exception.Push(ErrCode::IPC_EMODULE, {
                { ErrCode::IPC_EFDTYPE, "Bad file descriptor type" },
                { ErrCode::IPC_EUNKNOWEVENT, "Unknow select event" },
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
