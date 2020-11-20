#ifndef __CRYPT_RETURN_H__
#define __CRYPT_RETURN_H__

#include "return.h"

namespace infra::crypt {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        CRYPT_EMODULE = CRYPT_ERROR_CODE_MODULE,

        //snowflake
        CRYPT_SNOWFLAKE_INIT,
        CRYPT_SNOWFLAKE_EOUTBOUND,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::CRYPT_EMODULE)) {
            _exception.Push(ErrCode::CRYPT_EMODULE, {
                { ErrCode::CRYPT_SNOWFLAKE_INIT, "" },
                { ErrCode::CRYPT_SNOWFLAKE_EOUTBOUND, "" }
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
