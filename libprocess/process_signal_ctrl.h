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

#include "object.h"

//#include "mutex/thread_rw_lock.h"
#include "process_return.h"
#include "process_signal.h"
#include "process_signal_set.h"
#include "process_signal_action.h"
#include "process_signal_mask_type.h"

namespace infra::process {

/**
* @brief - Process level signal ctrl.
*/
class SignalCtrl : virtual public base::Object {
public:
//    thread::mutex::ThreadRWLock op_lock;                               ///< Muti thread lock
public:
    /**
    * @brief GetSignalAction - Get the action of the current corresponding signal.
    *
    * @param [sig] - Signal.
    *
    * @returns  Signal action.
    */
    SignalAction GetSignalAction(Signal& sig);
    SignalAction GetSignalAction(Signal&& sig);
    /**
    * @brief GetSignalMask - Get current mask signal set.
    *
    * @returns  Signal set.
    */
    SignalSet GetSignalMask();

    /**
    * @brief Register - Register signal action for designation signal.
    *
    * @param [sig] - Designation signal.
    * @param [action] - Signal action.
    *
    * @returns  Return.
    */
    Return Register(Signal& sig, SignalAction& action);
    Return Register(Signal&& sig, SignalAction&& action);
    /**
    * @brief Register - Register signal action for designation signal.
    *
    * @param [sig] - Designation signal.
    * @param [new_action] - New Signal action.
    * @param [old_action] - Old Signal action.
    *
    * @returns  Return.
    */
    Return Register(Signal& sig, SignalAction& new_action, SignalAction& old_action);
    Return Register(Signal&& sig, SignalAction&& new_action, SignalAction&& old_action);
    /**
    * @brief UnRegister - Unregister all signal.(change to default).
    *
    * @returns  Return.
    */
    Return UnRegister();
    /**
    * @brief UnRegister - Unregister signal(change to default).
    *
    * @param [sig] - Signal.
    *
    * @returns  Return.
    */
    Return UnRegister(Signal& sig);
    Return UnRegister(Signal&& sig);
    /**
    * @brief UnRegister - Unregister signal(change to default).
    *
    * @param [sig] - Signal.
    * @param [old_action] - Old signal action.
    *
    * @returns  Return.
    */
    Return UnRegister(Signal& sig, SignalAction& old_action);
    Return UnRegister(Signal&& sig, SignalAction&& old_action);
    /**
    * @brief Revert - Restore last time action for all signal.
    *
    * @returns  Return.
    */
    Return Revert();
    /**
    * @brief Revert - Restore last time action for the signal.
    *
    * @param [sig] - Signal.
    *
    * @returns  Return.
    */
    Return Revert(Signal& sig);
    Return Revert(Signal&& sig);
    
    /**
    * @brief Mask - Mask all signal.
    *
    * @returns  Return.
    */
    Return Mask();
    /**
    * @brief Mask - Append mask new signal set.
    *
    * @param [set] - New signal set.
    *
    * @returns  Return.
    */
    Return Mask(SignalSet& set);
    Return Mask(SignalSet&& set);
    /**
    * @brief Mask - Append mask new signal set.
    *
    * @param [new_set] - New signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  Return.
    */
    Return Mask(SignalSet& new_set, SignalSet& old_set);
    Return Mask(SignalSet&& new_set, SignalSet&& old_set);
    /**
    * @brief MaskReplace - Mask all signal
    *
    * @returns  Return.
    */
    Return MaskReplace();
    /**
    * @brief MaskReplace - Replace current mask signal by new mask signal set.
    *
    * @param [set] - New mask set.
    *
    * @returns  Return.
    */
    Return MaskReplace(SignalSet& set);
    Return MaskReplace(SignalSet&& set);
    /**
    * @brief MaskReplace - Replace current mask signal by new mask signal set.
    *
    * @param [new_set] - New signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  Return.
    */
    Return MaskReplace(SignalSet& new_set, SignalSet& old_set);
    Return MaskReplace(SignalSet&& new_set, SignalSet&& old_set);
    /**
    * @brief UnMask - Unmask all signal.
    *
    * @returns  Return.
    */
    Return UnMask();
    /**
    * @brief UnMask - Unmask signal set.
    *
    * @param [set] - Signal set.
    *
    * @returns  Return.
    */
    Return UnMask(SignalSet& set);
    Return UnMask(SignalSet&& set);
    /**
    * @brief UnMask - Unmask signal set.
    *
    * @param [new_set] - Signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  Return.
    */
    Return UnMask(SignalSet& new_set, SignalSet& old_set);
    Return UnMask(SignalSet&& new_set, SignalSet&& old_set);
    /**
    * @brief MaskRevert - Restore last time mask signal set.
    *
    * @returns  Return.
    */
    Return MaskRevert();

    static Return SendSignal(pid_t pid,Signal& sig);

    /**
    * @brief getInstance - Get Signal ctrl class instance.
    *
    * @returns  Pointer of instance.
    */
    static SignalCtrl* getInstance();
private:
    SignalCtrl();
    ~SignalCtrl();

    std::map<Signal, SignalAction> register_map_;         ///< Current register signal map.
    std::map<Signal, SignalAction> last_register_map_;    ///< Last time register signal map.

    SignalSet mask_set_;         ///< Current mask signal set.
    SignalSet last_mask_set_;    ///< Last time mask signal set.

    static SignalCtrl* pInstance;    ///< Signal instance pointer.

    Return _register_signal(Signal&& sig, SignalAction&& new_action, SignalAction&& old_action);
    Return _mask_signal(SignalMaskType how, SignalSet&& new_set, SignalSet&& old_set);
};

}

#endif
