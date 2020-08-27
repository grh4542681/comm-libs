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

#include "object.h"

#include "process_signal_set.h"

namespace infra::process {

class Signal;

/**
* @brief - Signal callback action
*/
class SignalAction : virtual public base::Object {
public:
    friend class Signal;
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
    * @brief SignalAction - Constructor.
    */
    SignalAction();
    /**
    * @brief SignalAction - Copy constructor.
    *
    * @param [other] - Instance ref.
    */
    SignalAction(const SignalAction& other);
    ~SignalAction();

    SignalAction& operator=(const SignalAction& other);

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
    SignalAction& SetCallback(SignalCallback callback);
    /**
    * @brief SetCallback - Set signal callback function.
    *
    * @param [callback] - Callback function pointer.
    *
    * @returns  Self ref.
    */
    SignalAction& SetCallback(SignalCallback2 callback);
    /**
    * @brief SetMaskset - Set signal mask set.
    *
    * @param [set] - Signal mask set ref.
    *
    * @returns  Self ref.
    */
    SignalAction& SetMaskset(SignalSet& set);
    SignalAction& SetMaskset(SignalSet&& set);
private:
    SignalAction(struct sigaction* action);
    struct sigaction action_;   ///< Linux sigaction.
};

}

#endif
