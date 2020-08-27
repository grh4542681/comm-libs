/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-17 11:22
 * @file     : process_signal.h
 * @brief    : Linux signal Encapsulation.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PROCESS_SIGNAL_ID_H__
#define __PROCESS_SIGNAL_ID_H__

#include <string>

#include "object.h"

namespace infra::process {

/**
* @brief - Linux signal Encapsulation.
*/
class SignalId : virtual public base::Object {
public:
    friend class SignalSet;
    friend class Signal;
public:
    /**
    * @brief SignalId - Constructor.
    */
    SignalId();
    /**
    * @brief SignalId - Constructor.
    *
    * @param [sig] - Linux signal.
    */
    SignalId(int sig);
    /**
    * @brief SignalId - Copy constructor.
    *
    * @param [other] - Instance referance,
    */
    SignalId(const SignalId& other);
    ~SignalId();

    SignalId& operator= (const SignalId& other);
    bool operator< (const SignalId& other) const;
private:
    int sig_;   ///< Linux signal.
    std::string sig_name_;
};

}

#endif
