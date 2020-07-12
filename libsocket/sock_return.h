/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 09:59
 * @file     : sock_return.h
 * @brief    : Socket return value header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __SOCK_RETURN_H__
#define __SOCK_RETURN_H__

#include "return.h"

namespace sock{

/**
* @brief - Socket api return value.
*/
class SockRet : public ret::Return {
public:
    enum ECode{
        SOCK_EBASE = SOCK_ERROR_CODE_BASE,
        SOCK_LINKDOWN,      ///< link down.
        SOCK_EADDRESS,
    };
public:
    static ECodeMapType ECodeMap;
public:
    SockRet(int err_code = 0) : ret::Return(err_code) {
        err_code_vec_.push_back(&SockRet::ECodeMap);
    }
    SockRet(SockRet& other) : ret::Return(other) { }
    ~SockRet() { };
public:
    SockRet& operator=(const int err_code) {
        Return::operator=(err_code);
        return *this;
    }   
    SockRet& operator=(const SockRet& ret) {
        Return::operator=(ret);
        return *this;
    }   
    SockRet& operator=(const SockRet&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
