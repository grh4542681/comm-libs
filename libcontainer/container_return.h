#ifndef __CONTAINER_RETURN_H__
#define __CONTAINER_RETURN_H__

#include "return.h"

namespace container {

class ContainerRet : public ret::Return {
public:
    enum ECode{
        CONTAINER_EBASE = CONTAINER_ERROR_CODE_BASE,
        BM_EOUTBOUND,

        NT_EINDEXCROSS,
        NT_ENOTFOUND,

        TT_ENOTFOUND,
        TT_EKEY,
        TT_EINDEX,
    };
public:
    static ECodeMapType ECodeMap;
public:
    ContainerRet(int err_code = 0) : ret::Return(err_code) {
        err_code_vec_.push_back(&ContainerRet::ECodeMap);
    }
    ContainerRet(ContainerRet& other) : ret::Return(other) { }
    ~ContainerRet() { };
public:
    ContainerRet& operator=(const int err_code) {
        Return::operator=(err_code);
        return *this;
    }   
    ContainerRet& operator=(const ContainerRet& ret) {
        Return::operator=(ret);
        return *this;
    }   
    ContainerRet& operator=(const ContainerRet&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
