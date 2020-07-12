/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-18 04:19
 * @file     : process_signal_ctrl.h
 * @brief    : Process level signal ctrl.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PROCESS_SIGNAL_CTRL_H__
#define __PROCESS_SIGNAL_CTRL_H__

#include <map>

#include "mempool.h"
#include "mutex/thread_rw_lock.h"
#include "process_return.h"
#include "process_signal.h"
#include "process_signal_set.h"
#include "process_signal_action.h"
#include "process_signal_mask_type.h"

namespace process::signal {

/**
* @brief - Process level signal ctrl.
*/
class ProcessSignalCtrl {
public:
    thread::mutex::ThreadRWLock op_lock;                               ///< Muti thread lock
public:
    /**
    * @brief GetSignalAction - Get the action of the current corresponding signal.
    *
    * @param [sig] - Signal.
    *
    * @returns  Signal action.
    */
    ProcessSignalAction GetSignalAction(ProcessSignal& sig);
    ProcessSignalAction GetSignalAction(ProcessSignal&& sig);
    /**
    * @brief GetSignalMask - Get current mask signal set.
    *
    * @returns  Signal set.
    */
    ProcessSignalSet GetSignalMask();

    /**
    * @brief Register - Register signal action for designation signal.
    *
    * @param [sig] - Designation signal.
    * @param [action] - Signal action.
    *
    * @returns  ProcessRet.
    */
    ProcessRet Register(ProcessSignal& sig, ProcessSignalAction& action);
    ProcessRet Register(ProcessSignal&& sig, ProcessSignalAction&& action);
    /**
    * @brief Register - Register signal action for designation signal.
    *
    * @param [sig] - Designation signal.
    * @param [new_action] - New Signal action.
    * @param [old_action] - Old Signal action.
    *
    * @returns  ProcessRet.
    */
    ProcessRet Register(ProcessSignal& sig, ProcessSignalAction& new_action, ProcessSignalAction& old_action);
    ProcessRet Register(ProcessSignal&& sig, ProcessSignalAction&& new_action, ProcessSignalAction&& old_action);
    /**
    * @brief UnRegister - Unregister all signal.(change to default).
    *
    * @returns  ProcessRet.
    */
    ProcessRet UnRegister();
    /**
    * @brief UnRegister - Unregister signal(change to default).
    *
    * @param [sig] - Signal.
    *
    * @returns  ProcessRet.
    */
    ProcessRet UnRegister(ProcessSignal& sig);
    ProcessRet UnRegister(ProcessSignal&& sig);
    /**
    * @brief UnRegister - Unregister signal(change to default).
    *
    * @param [sig] - Signal.
    * @param [old_action] - Old signal action.
    *
    * @returns  ProcessRet.
    */
    ProcessRet UnRegister(ProcessSignal& sig, ProcessSignalAction& old_action);
    ProcessRet UnRegister(ProcessSignal&& sig, ProcessSignalAction&& old_action);
    /**
    * @brief Revert - Restore last time action for all signal.
    *
    * @returns  ProcessRet.
    */
    ProcessRet Revert();
    /**
    * @brief Revert - Restore last time action for the signal.
    *
    * @param [sig] - Signal.
    *
    * @returns  ProcessRet.
    */
    ProcessRet Revert(ProcessSignal& sig);
    ProcessRet Revert(ProcessSignal&& sig);
    
    /**
    * @brief Mask - Mask all signal.
    *
    * @returns  ProcessRet.
    */
    ProcessRet Mask();
    /**
    * @brief Mask - Append mask new signal set.
    *
    * @param [set] - New signal set.
    *
    * @returns  ProcessRet.
    */
    ProcessRet Mask(ProcessSignalSet& set);
    ProcessRet Mask(ProcessSignalSet&& set);
    /**
    * @brief Mask - Append mask new signal set.
    *
    * @param [new_set] - New signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  ProcessRet.
    */
    ProcessRet Mask(ProcessSignalSet& new_set, ProcessSignalSet& old_set);
    ProcessRet Mask(ProcessSignalSet&& new_set, ProcessSignalSet&& old_set);
    /**
    * @brief MaskReplace - Mask all signal
    *
    * @returns  ProcessRet.
    */
    ProcessRet MaskReplace();
    /**
    * @brief MaskReplace - Replace current mask signal by new mask signal set.
    *
    * @param [set] - New mask set.
    *
    * @returns  ProcessRet.
    */
    ProcessRet MaskReplace(ProcessSignalSet& set);
    ProcessRet MaskReplace(ProcessSignalSet&& set);
    /**
    * @brief MaskReplace - Replace current mask signal by new mask signal set.
    *
    * @param [new_set] - New signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  ProcessRet.
    */
    ProcessRet MaskReplace(ProcessSignalSet& new_set, ProcessSignalSet& old_set);
    ProcessRet MaskReplace(ProcessSignalSet&& new_set, ProcessSignalSet&& old_set);
    /**
    * @brief UnMask - Unmask all signal.
    *
    * @returns  ProcessRet.
    */
    ProcessRet UnMask();
    /**
    * @brief UnMask - Unmask signal set.
    *
    * @param [set] - Signal set.
    *
    * @returns  ProcessRet.
    */
    ProcessRet UnMask(ProcessSignalSet& set);
    ProcessRet UnMask(ProcessSignalSet&& set);
    /**
    * @brief UnMask - Unmask signal set.
    *
    * @param [new_set] - Signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  ProcessRet.
    */
    ProcessRet UnMask(ProcessSignalSet& new_set, ProcessSignalSet& old_set);
    ProcessRet UnMask(ProcessSignalSet&& new_set, ProcessSignalSet&& old_set);
    /**
    * @brief MaskRevert - Restore last time mask signal set.
    *
    * @returns  ProcessRet.
    */
    ProcessRet MaskRevert();

    static ProcessRet SendSignal(pid_t pid,ProcessSignal& sig);

    /**
    * @brief getInstance - Get Signal ctrl class instance.
    *
    * @returns  Pointer of instance.
    */
    static ProcessSignalCtrl* getInstance();
private:
    ProcessSignalCtrl();
    ~ProcessSignalCtrl();

    mempool::Mempool* mempool_;                                         ///< Mempool pointer.
    std::map<ProcessSignal, ProcessSignalAction> register_map_;         ///< Current register signal map.
    std::map<ProcessSignal, ProcessSignalAction> last_register_map_;    ///< Last time register signal map.

    ProcessSignalSet mask_set_;         ///< Current mask signal set.
    ProcessSignalSet last_mask_set_;    ///< Last time mask signal set.

    static ProcessSignalCtrl* pInstance;    ///< Signal instance pointer.

    ProcessRet _register_signal(ProcessSignal&& sig, ProcessSignalAction&& new_action, ProcessSignalAction&& old_action);
    ProcessRet _mask_signal(SignalMaskType how, ProcessSignalSet&& new_set, ProcessSignalSet&& old_set);
};

}

#endif
