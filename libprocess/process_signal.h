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
#ifndef __PROCESS_SIGNAL_H__
#define __PROCESS_SIGNAL_H__

#include <string>

namespace infra::process {

/**
* @brief - Linux signal Encapsulation.
*/
class Signal {
public:
    friend class SignalSet;
    friend class SignalCtrl;
public:
    /**
    * @brief Signal - Constructor.
    */
    Signal();
    /**
    * @brief Signal - Constructor.
    *
    * @param [sig] - Linux signal.
    */
    Signal(int sig);
    /**
    * @brief Signal - Copy constructor.
    *
    * @param [other] - Instance referance,
    */
    Signal(const Signal& other);
    ~Signal();

    Signal& operator= (const Signal& other);
    bool operator< (const Signal& other) const;
private:
    int sig_;   ///< Linux signal.
    std::string sig_name_;
};

}

#endif
