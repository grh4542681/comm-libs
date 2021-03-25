/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-17 11:25
 * @file     : process_signal_set.h
 * @brief    : ProcessSignal set.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PROCESS_SIGNAL_SET_H__
#define __PROCESS_SIGNAL_SET_H__

#include "signal.h"

#include "process_return.h"
#include "process_signal.h"

namespace infra::infra::thread::signal {
class ThreadSignalCtrl;
}

namespace infra::infra::process::signal {

class ProcessSignalCtrl;
class ProcessSignalAction;

/**
* @brief - Signal set.
*/
class ProcessSignalSet {
public:
    friend class ProcessSignalAction;
    friend class ProcessSignalCtrl;
    friend class thread::signal::ThreadSignalCtrl;
public:
    ProcessSignalSet();
    ProcessSignalSet(const ProcessSignalSet& other);
    ~ProcessSignalSet();

    const ProcessSignalSet& operator= (const ProcessSignalSet& other);

    bool IsEmpty();

    /**
    * @brief AddSig - Add a signal.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  ProcessRet.
    */
    ProcessRet AddSig(ProcessSignal& sig);
    ProcessRet AddSig(ProcessSignal&& sig);
    /**
    * @brief AddAll - Add all signal.
    *
    * @returns  ProcessRet.
    */
    ProcessRet AddAll();
    /**
    * @brief DelSig - Delete a signal from set.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  ProcessRet.
    */
    ProcessRet DelSig(ProcessSignal& sig);
    ProcessRet DelSig(ProcessSignal&& sig);
    /**
    * @brief DelAll - Clean set.
    *
    * @returns  ProcessRet.
    */
    ProcessRet DelAll();
    /**
    * @brief HasSig - Determine if the signal is in the map.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  bool.
    */
    bool HasSig(ProcessSignal& sig);
    bool HasSig(ProcessSignal&& sig);

    sigset_t* GetSigset();

private:
    sigset_t set_;  ///< Linux signal set.
    bool empty_flag_;
};

}

#endif
