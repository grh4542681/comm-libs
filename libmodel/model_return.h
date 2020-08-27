#ifndef __MODEL_RETURN_H__
#define __MODEL_RETURN_H__

#include "return.h"

namespace infra::model {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        MODEL_EMODULE = MODEL_ERROR_CODE_MODULE,

        //efsm
        MODEL_EFSM_EVENT_ENOTEXIST,
        MODEL_EFSM_EVENT_ETRANSEXIST,
        MODEL_EFSM_STATE_ENOTEXIST,
        MODEL_EFSM_STATE_ETRANSEXIST,
        MODEL_EFSM_TRANS_ENOTEXIST,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::MODEL_EMODULE)) {
            _exception.Push(ErrCode::MODEL_EMODULE, {
                { ErrCode::MODEL_EFSM_EVENT_ENOTEXIST, "EFSM event does not exist" },
                { ErrCode::MODEL_EFSM_EVENT_ETRANSEXIST, "EFSM event already bind in trans" },
                { ErrCode::MODEL_EFSM_STATE_ENOTEXIST, "EFSM state does not exist" },
                { ErrCode::MODEL_EFSM_STATE_ETRANSEXIST, "EFSM state already bind in trans" },
                { ErrCode::MODEL_EFSM_TRANS_ENOTEXIST, "EFSM trans does not exist" }

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
