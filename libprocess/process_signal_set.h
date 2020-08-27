/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-17 11:25
 * @file     : process_signal_set.h
 * @brief    : Signal set.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PROCESS_SIGNAL_SET_H__
#define __PROCESS_SIGNAL_SET_H__

#include <signal.h>

#include "object.h"

#include "process_return.h"
#include "process_signal_id.h"

namespace infra::process {

class Signal;
class SignalAction;

/**
* @brief - Signal set.
*/
class SignalSet : virtual public base::Object {
public:
    friend class SignalAction;
    friend class Signal;
public:
    SignalSet();
    SignalSet(const SignalSet& other);
    ~SignalSet();

    const SignalSet& operator= (const SignalSet& other);

    bool IsEmpty();

    /**
    * @brief AddSig - Add a signal.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  Return.
    */
    Return AddSig(SignalId& sig);
    Return AddSig(SignalId&& sig);
    /**
    * @brief AddAll - Add all signal.
    *
    * @returns  Return.
    */
    Return AddAll();
    /**
    * @brief DelSig - Delete a signal from set.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  Return.
    */
    Return DelSig(SignalId& sig);
    Return DelSig(SignalId&& sig);
    /**
    * @brief DelAll - Clean set.
    *
    * @returns  Return.
    */
    Return DelAll();
    /**
    * @brief HasSig - Determine if the signal is in the map.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  bool.
    */
    bool HasSig(SignalId& sig);
    bool HasSig(SignalId&& sig);

    sigset_t* GetSigset();

private:
    sigset_t set_;  ///< Linux signal set.
    bool empty_flag_;
};

}

#endif
