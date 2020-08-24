#ifndef __PROCESS_RETURN_H__
#define __PROCESS_RETURN_H__

#include "return.h"

namespace infra::process {

class Return : virtual public base::Object, public base::Return {
public:
    enum ErrCode{
        EDEFAULE,
        PROCESS_EMODULE = PROCESS_ERROR_CODE_MODULE,

    //process common
        PROCESS_EMEMORY,

        PROCESS_EINVALFD,
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

    //process parent
        PROCESS_PARENT_EEXIST,
        PROCESS_PARENT_ENOTEXIST,
        PROCESS_PARENT_EINSERT,
        PROCESS_PARENT_EDELETE,

    //process child
        PROCESS_CHILD_EEXIST,
        PROCESS_CHILD_ENOTEXIST,
        PROCESS_CHILD_EINSERT,
        PROCESS_CHILD_EDELETE,
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::PROCESS_EMODULE)) {
            _exception.Push(ErrCode::PROCESS_EMODULE, {
                { ErrCode::PROCESS_EACCES, "No access" }
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
