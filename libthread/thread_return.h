/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 09:04
 * @file     : thread_return.h
 * @brief    : Thread return value header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __THREAD_RETURN_H__
#define __THREAD_RETURN_H__

#include "return.h"

namespace thread{

class ThreadRet : public ret::Return {
public:
    enum ECode{
        THREAD_EBASE = THREAD_ERROR_CODE_BASE,

    //thread info
        THREAD_EREGISTER,
        THREAD_EUNREGISTER,

    //thread single
        THREAD_ERUNNING,

    //thread signal
        THREAD_ESIGNALMASKTYPE,
        THREAD_ESIGNALMASK,
    };
public:
    static ECodeMapType ECodeMap;
public:
    ThreadRet(int err_code = 0) : ret::Return(err_code) {
        err_code_vec_.push_back(&ThreadRet::ECodeMap);
    }
    ThreadRet(ThreadRet& other) : ret::Return(other) { }
    ~ThreadRet() { };
public:
    ThreadRet& operator=(const int err_code) {
        Return::operator=(err_code);
        return *this;
    }   
    ThreadRet& operator=(const ThreadRet& ret) {
        Return::operator=(ret);
        return *this;
    }   
    ThreadRet& operator=(const ThreadRet&& ret) {
        Return::operator=(ret);
        return *this;
    }
};

}

#endif
