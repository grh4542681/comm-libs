#ifndef __CONTAINER_RETURN_H__
#define __CONTAINER_RETURN_H__

#include "return.h"

namespace infra::container {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        CONTAINER_EMODULE = CONTAINER_ERROR_CODE_MODULE,

        //bitmap
        CONTAINER_BITMAP_INIT,
        CONTAINER_BITMAP_EOUTBOUND,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::CONTAINER_EMODULE)) {
            _exception.Push(ErrCode::CONTAINER_EMODULE, {
                { ErrCode::CONTAINER_BITMAP_INIT, "" },
                { ErrCode::CONTAINER_BITMAP_EOUTBOUND, "" }
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
