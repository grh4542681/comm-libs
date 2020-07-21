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

#include "signal.h"

#include "process_return.h"
#include "process_signal.h"

namespace infra::process {

class SignalCtrl;
class SignalAction;

/**
* @brief - Signal set.
*/
class SignalSet {
public:
    friend class SignalAction;
    friend class SignalCtrl;
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
    Return AddSig(Signal& sig);
    Return AddSig(Signal&& sig);
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
    Return DelSig(Signal& sig);
    Return DelSig(Signal&& sig);
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
    bool HasSig(Signal& sig);
    bool HasSig(Signal&& sig);

    sigset_t* GetSigset();

private:
    sigset_t set_;  ///< Linux signal set.
    bool empty_flag_;
};

}

#endif
