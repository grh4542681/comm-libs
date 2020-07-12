/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 09:01
 * @file     : parser_return.h
 * @brief    : Parser return value header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PARSER_RETURN_H__
#define __PARSER_RETURN_H__

#include "return.h"

namespace parser{

/**
* @brief - Parser return value.
*/
class ParserRet : public ret::Return {
public:
    enum ECode{
        PARSER_EBASE = PARSER_ERROR_CODE_BASE,
        PARSER_ENOTFOUND,
        PARSER_ELOCK,
        PARSER_EGETRLOCK,
        PARSER_EGETWLOCK,
        PARSER_EUNLOCK,
        PARSER_ETYPE,
        PARSER_ESPACE,
        PARSER_ESERIAL,
        PARSER_EFORMAT,
    };
public:
    static ECodeMapType ECodeMap;
public:
    ParserRet(int err_code = 0) : ret::Return(err_code) {
        err_code_vec_.push_back(&ParserRet::ECodeMap);
    }
    ParserRet(ParserRet& other) : ret::Return(other) { }
    ~ParserRet() { };
public:
    ParserRet& operator=(const int err_code) {
        Return::operator=(err_code);
        return *this;
    }   
    ParserRet& operator=(const ParserRet& ret) {
        Return::operator=(ret);
        return *this;
    }   
    ParserRet& operator=(const ParserRet&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
