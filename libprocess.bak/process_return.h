#ifndef __PROCESS_RETURN_H__
#define __PROCESS_RETURN_H__

#include "return.h"

namespace infra::process {

class ProcessRet : public ret::Return {
public:
    enum ECode{
        PROCESS_EBASE = PROCESS_ERROR_CODE_BASE,
    //process common
        PROCESS_EMEMORY,

    // process info
        PROCESS_EACCES,
        PROCESS_EFAULT,
        PROCESS_EINVAL,
        PROCESS_EIO,
        PROCESS_ELOOP,
        PROCESS_ENAMETOOLONG,
        PROCESS_ENOENT,
        PROCESS_ENOMEM,
        PROCESS_ENOTDIR,
        PROCESS_ETHREADDUP,
        PROCESS_ETHREADADD,
        PROCESS_ETHREADNOTFOUND,
        PROCESS_EPROCDUP,
        PROCESS_EPROCADD,
        PROCESS_EPROCNOTFOUND,
        PROCESS_ECMDLINE,
        PROCESS_ENOCHILD,

    //process signal
        PROCESS_ESIGINVAL,
        PROCESS_EADDSIGNAL,
        PROCESS_ESIGNALREG,
        PROCESS_ESIGNALNOTFOUND,
        PROCESS_ESIGNALMASK,
        PROCESS_ESIGNALMASKTYPE,

    //process single
        PROCESS_ECALLABLE,
        PROCESS_EFORK,
        PROCESS_EFIFOPAIR,

    //process config
        PROCESS_ERAWCONFIG,
        PROCESS_ECONFIG,
    };
public:
    static ECodeMapType ECodeMap;
public:
    ProcessRet(int err_code = 0) : ret::Return(err_code) {
        err_code_vec_.push_back(&ProcessRet::ECodeMap);
    }
    ProcessRet(const ProcessRet& other) : ret::Return(other) { }
    ~ProcessRet() { };
public:
    ProcessRet& operator=(const int err_code) {
        Return::operator=(err_code);
        return *this;
    }   
    ProcessRet& operator=(const ProcessRet& ret) {
        Return::operator=(ret);
        return *this;
    }   
    ProcessRet& operator=(const ProcessRet&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
