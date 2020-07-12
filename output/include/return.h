#ifndef __RETURN_H__
#define __RETURN_H__

#include <string>
#include <map>
#include <errno.h>
#include <string.h>

#include "err_code.h"

#define EDEFAULE UNKNOW = -2, \
                 ERROR = -1, \
                 SUCCESS = 0

namespace base {

class Exception {
public:
    typedef int ECodeType;
    typedef std::string EStringType;
    typedef std::map<ECodeType, EStringType> EMapType;

    static Exception& Instance() {
        static Exception instance;
        return instance;
    }

    bool ModuleExist(int module) {
        auto errCodeMapIt = _errCodeMap.find(module);
        return (errCodeMapIt == _errCodeMap.end() ? false : true);
    }
    void Push(int module, EMapType&& emap) {
        if (!(ModuleExist(module))) {
            _errCodeMap.insert({module, emap});
        }
    }
    EStringType Search(int module, int ecode) {
        auto errCodeMapIt = _errCodeMap.find(module);
        if (errCodeMapIt != _errCodeMap.end()) {
            auto eMapIt = errCodeMapIt->second.find(ecode);
            if (eMapIt != errCodeMapIt->second.end()) {
                return eMapIt->second;
            }
        }
        return "";
    }
    EStringType Search(int ecode) {
        for (auto errCodeMapIt : _errCodeMap) {
            auto eMapIt = errCodeMapIt.second.find(ecode);
            if (eMapIt != errCodeMapIt.second.end()) {
                return eMapIt->second;
            }
        }
        return "";
    }

    ~Exception() { }
    std::map<int, EMapType> _errCodeMap;
private:
    Exception() { }
    Exception(const Exception&);
    Exception& operator= (const Exception&);
};

class Return {
public:
    enum ErrCode : int {
        EDEFAULE,
        EMODULE = PUBLIC_ERROR_CODE_MODULE,
        EBADARGS,
        EMAP,
        ELIST,
        EVECTOR,
        EINIT,
        ETIMEOUT,
        ESUBCLASS,
    };
public:
    Return(int ecode) : _ecode(ecode), _exception(Exception::Instance()) {
        if (!_exception.ModuleExist(Return::ErrCode::EMODULE)) {
            _exception.Push(Return::ErrCode::EMODULE, {
                { Return::ErrCode::UNKNOW, "unknow error" },
                { Return::ErrCode::ERROR, "error" },
                { Return::ErrCode::SUCCESS, "success" },
                { Return::ErrCode::EBADARGS, "bad arguments" },
                { Return::ErrCode::EMAP, "std map error" },
                { Return::ErrCode::ELIST, "std list error" },
                { Return::ErrCode::EVECTOR, "bad arguments" },
                { Return::ErrCode::EINIT, "inilazition error" },
                { Return::ErrCode::ETIMEOUT, "time out" },
                { Return::ErrCode::ESUBCLASS, "can not find subclass" }
            });
        }
    }
    Return(const Return& other) : _exception(Exception::Instance()) {
        _ecode = other._ecode;
    }
    virtual ~Return() { }

    virtual int ECode() const {
        return _ecode;
    }

    virtual const std::string EString() {
        std::string emgs = "";
        if (_ecode >= 0 && _ecode < ERROR_CODE_BASE) {
            emgs.assign(strerror(_ecode));
        } else {
            emgs = _exception.Search(_ecode);
        }
        return emgs;
    }

public:
    Return& operator=(const int ecode) {
        _ecode = ecode;
        return *this;
    }
    Return& operator=(const Return& ret) {
        _ecode = ret.ECode();
        return *this;
    }
    Return& operator=(const Return&& ret) {
        _ecode = ret.ECode();
        return *this;
    }
    
    bool operator==(int ecode) {
        return (_ecode == ecode);
    }
    bool operator==(Return& ret) {
        return (_ecode == ret.ECode());
    }
    bool operator==(Return&& ret) {
        return (_ecode == ret.ECode());
    }

    bool operator!=(int ecode) {
        return (_ecode != ecode);
    }
    bool operator!=(Return& ret) {
        return (_ecode != ret.ECode());
    }
    bool operator!=(Return&& ret) {
        return (_ecode != ret.ECode());
    }

protected:
    int _ecode;
    Exception& _exception;
};

}

#endif
