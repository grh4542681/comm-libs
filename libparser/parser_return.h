#ifndef __PARSER_RETURN_H__
#define __PARSER_RETURN_H__

#include "return.h"

namespace infra::parser {

class Return : public base::Return {
public:
    enum ErrCode{
        EDEFAULT,
        PARSER_EMODULE = PARSER_ERROR_CODE_MODULE,
        PARSER_ENOTFOUND,
        PARSER_ELOCK,
        PARSER_EGETRLOCK,
        PARSER_EGETWLOCK,
        PARSER_EUNLOCK,
        PARSER_ETYPE,
        PARSER_ESPACE,
        PARSER_ESERIAL,
        PARSER_EFORMAT
    };
public:
    Return(int ecode = 0) : base::Return(ecode) {
        if (!_exception.ModuleExist(ErrCode::PARSER_EMODULE)) {
            _exception.Push(ErrCode::PARSER_EMODULE, {
                { ErrCode::PARSER_ESPACE, "No space" },
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
