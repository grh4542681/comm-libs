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

namespace infra::process::signal {

/**
* @brief - Linux signal Encapsulation.
*/
class ProcessSignal {
public:
    friend class ProcessSignalSet;
    friend class ProcessSignalCtrl;
public:
    /**
    * @brief ProcessSignal - Constructor.
    */
    ProcessSignal();
    /**
    * @brief ProcessSignal - Constructor.
    *
    * @param [sig] - Linux signal.
    */
    ProcessSignal(int sig);
    /**
    * @brief ProcessSignal - Copy constructor.
    *
    * @param [other] - Instance referance,
    */
    ProcessSignal(const ProcessSignal& other);
    ~ProcessSignal();

    ProcessSignal& operator= (const ProcessSignal& other);
    bool operator< (const ProcessSignal& other) const;
private:
    int sig_;   ///< Linux signal.
    std::string sig_name_;
};

}

#endif
