/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-18 03:55
 * @file     : process_signal_action.h
 * @brief    : Signal callback
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PROCESS_SIGNAL_ACTION_H__
#define __PROCESS_SIGNAL_ACTION_H__

#include "process_signal_set.h"

namespace infra::process::signal {

class ProcessSignalCtrl;

/**
* @brief - Signal callback action
*/
class ProcessSignalAction {
public:
    friend class ProcessSignalCtrl;
    /**
    * @brief  - Signal callback function type.
    *
    * @param [] - signal.
    */
    typedef void (*SignalCallback)(int);
    /**
    * @brief  - Signal callback function type.
    *
    * @param [] - signal
    * @param [siginfo_t] - siginfo
    * @param [] - ucontext_t
    *
    */
    typedef void (*SignalCallback2)(int, siginfo_t*, void*);
public:
    /**
    * @brief ProcessSignalAction - Constructor.
    */
    ProcessSignalAction();
    /**
    * @brief ProcessSignalAction - Copy constructor.
    *
    * @param [other] - Instance ref.
    */
    ProcessSignalAction(const ProcessSignalAction& other);
    ~ProcessSignalAction();

    ProcessSignalAction& operator=(const ProcessSignalAction& other);

    /**
    * @brief IsDefault - This instance is not the default behavior.
    *
    * @returns  bool.
    */
    bool IsDefault();
    /**
    * @brief SetCallback - Set signal callback function.
    *
    * @param [callback] - Callback function pointer.
    *
    * @returns  Self ref.
    */
    ProcessSignalAction& SetCallback(SignalCallback callback);
    /**
    * @brief SetCallback - Set signal callback function.
    *
    * @param [callback] - Callback function pointer.
    *
    * @returns  Self ref.
    */
    ProcessSignalAction& SetCallback(SignalCallback2 callback);
    /**
    * @brief SetMaskset - Set signal mask set.
    *
    * @param [set] - Signal mask set ref.
    *
    * @returns  Self ref.
    */
    ProcessSignalAction& SetMaskset(ProcessSignalSet& set);
    ProcessSignalAction& SetMaskset(ProcessSignalSet&& set);
private:
    ProcessSignalAction(struct sigaction* action);
    struct sigaction action_;   ///< Linux sigaction.
};

}

#endif
