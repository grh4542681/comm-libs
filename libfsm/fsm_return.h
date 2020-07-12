#ifndef __FSM_RETURN_H__
#define __FSM_RETURN_H__

#include "return.h"

namespace fsm {

class FsmRet : public ret::Return {
public:
    enum ECode{
        FSM_EBASE = FSM_ERROR_CODE_BASE,

        FSM_EVENT_ENOTEXIST,
        FSM_EVENT_ETRANSEXIST,

        FSM_STATE_ENOTEXIST,
        FSM_STATE_ETRANSEXIST,
        FSM_STATE_EEXIT,
        FSM_STATE_EUPDATE,
        FSM_STATE_EENTER,

        FSM_TRANS_ENOTEXIST,
        FSM_TRANS_ECHECK,
        FSM_TRANS_ECOMPLETE,
    };
public:
    static ECodeMapType ECodeMap;
public:
    FsmRet(int err_code = 0) : ret::Return(err_code) {
        err_code_vec_.push_back(&FsmRet::ECodeMap);
    }
    FsmRet(FsmRet& other) : ret::Return(other) { }
    ~FsmRet() { };
public:
    FsmRet& operator=(const int err_code) {
        Return::operator=(err_code);
        return *this;
    }   
    FsmRet& operator=(const FsmRet& ret) {
        Return::operator=(ret);
        return *this;
    }   
    FsmRet& operator=(const FsmRet&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
