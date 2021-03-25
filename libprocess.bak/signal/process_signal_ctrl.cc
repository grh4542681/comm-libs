/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-18 10:57
 * @file     : process_signal_ctrl.cc
 * @brief    : Process signal ctrl.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#include <stddef.h>

#include "process_log.h"
#include "process_signal_ctrl.h"

namespace infra::process::signal {

ProcessSignalCtrl* ProcessSignalCtrl::pInstance = NULL;

ProcessSignalCtrl::ProcessSignalCtrl()
{
    mempool_ = mempool::Mempool::getInstance();
}

ProcessSignalCtrl::~ProcessSignalCtrl()
{

}

ProcessSignalCtrl* ProcessSignalCtrl::getInstance()
{
    if (!pInstance) {
        pInstance = new ProcessSignalCtrl();
    }
    return pInstance;
}

ProcessSignalAction ProcessSignalCtrl::GetSignalAction(ProcessSignal& sig)
{
    return GetSignalAction(std::move(sig));
}

ProcessSignalAction ProcessSignalCtrl::GetSignalAction(ProcessSignal&& sig)
{
    ProcessSignalAction action;
    auto it = register_map_.find(sig);
    if (it != register_map_.end()) {
        action = it->second;
    }
    return action;
}

ProcessSignalSet ProcessSignalCtrl::GetSignalMask()
{
    return mask_set_;
}

ProcessRet ProcessSignalCtrl::Register(ProcessSignal& sig, ProcessSignalAction& action)
{
    return Register(std::move(sig), std::move(action));
}

ProcessRet ProcessSignalCtrl::Register(ProcessSignal&& sig, ProcessSignalAction&& action)
{
    ProcessSignalAction old_action;
    return Register(std::move(sig), std::move(action), std::move(old_action));
}

ProcessRet ProcessSignalCtrl::Register(ProcessSignal& sig, ProcessSignalAction& new_action, ProcessSignalAction& old_action)
{
    return Register(std::move(sig), std::move(new_action), std::move(old_action));
}

ProcessRet ProcessSignalCtrl::Register(ProcessSignal&& sig, ProcessSignalAction&& new_action, ProcessSignalAction&& old_action)
{
    ProcessRet ret = ProcessRet::SUCCESS;
    if ((ret = _register_signal(std::move(sig), std::move(new_action), std::move(old_action))) != ProcessRet::SUCCESS) {
        return ret;
    }
    //std::pair<std::map<ProcessSignal, ProcessSignalAction>::iterator, bool> ret;
    register_map_.insert_or_assign(sig, new_action);
    last_register_map_.insert_or_assign(sig, old_action);
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessSignalCtrl::UnRegister()
{
    ProcessRet ret;
    ProcessSignal sig;
    std::map<ProcessSignal, ProcessSignalAction> register_map = register_map_;
    for (const auto& it : register_map) {
        sig = it.first;
        ret = UnRegister(std::move(sig));
        if ( ret != ProcessRet::SUCCESS) {
            return ret;
        }
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessSignalCtrl::UnRegister(ProcessSignal& sig)
{
    return UnRegister(std::move(sig));
}

ProcessRet ProcessSignalCtrl::UnRegister(ProcessSignal&& sig)
{
    ProcessSignalAction old_action;
    return UnRegister(std::move(sig), std::move(old_action));
}

ProcessRet ProcessSignalCtrl::UnRegister(ProcessSignal& sig, ProcessSignalAction& old_action)
{
    return UnRegister(std::move(sig), std::move(old_action));
}

ProcessRet ProcessSignalCtrl::UnRegister(ProcessSignal&& sig, ProcessSignalAction&& old_action)
{
    ProcessSignalAction default_action;
    return Register(std::move(sig), std::move(default_action), std::move(old_action));
}

ProcessRet ProcessSignalCtrl::Revert()
{
    ProcessRet ret;
    ProcessSignal sig;
    ProcessSignalAction action;
    std::map<ProcessSignal, ProcessSignalAction> register_map = last_register_map_;
    for (const auto& it : register_map) {
        sig = it.first;
        action = it.second;
        ret = Register(std::move(sig), std::move(action));
        if ( ret != ProcessRet::SUCCESS) {
            return ret;
        }
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessSignalCtrl::Revert(ProcessSignal& sig)
{
    return Revert(std::move(sig));
}

ProcessRet ProcessSignalCtrl::Revert(ProcessSignal&& sig)
{
    auto it = last_register_map_.find(sig);
    if (it == last_register_map_.end()) {
        return ProcessRet::PROCESS_ESIGNALNOTFOUND;
    }
    ProcessSignalAction action = it->second;
    return Register(std::move(sig), std::move(action));
}

ProcessRet ProcessSignalCtrl::Mask()
{
    ProcessSignalSet new_set;
    ProcessSignalSet old_set;
    new_set.AddAll();
    return Mask(new_set, old_set);
}

ProcessRet ProcessSignalCtrl::Mask(ProcessSignalSet& set)
{
    return Mask(std::move(set));
}

ProcessRet ProcessSignalCtrl::Mask(ProcessSignalSet&& set)
{
    ProcessSignalSet old_set;
    return Mask(set, old_set);
}

ProcessRet ProcessSignalCtrl::Mask(ProcessSignalSet& new_set, ProcessSignalSet& old_set)
{
    return Mask(std::move(new_set), std::move(old_set));
}

ProcessRet ProcessSignalCtrl::Mask(ProcessSignalSet&& new_set, ProcessSignalSet&& old_set)
{
    ProcessRet ret = _mask_signal(SignalMaskType::APPEND, std::move(new_set), std::move(old_set));
    if (ret != ProcessRet::SUCCESS) {
        return ret;
    }
    last_mask_set_ = old_set;
    ret = _mask_signal(SignalMaskType::GETMASK, std::move(mask_set_), std::move(mask_set_));
    if (ret != ProcessRet::SUCCESS) {
        return ret;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessSignalCtrl::MaskReplace(ProcessSignalSet& set)
{
    return MaskReplace(std::move(set));
}

ProcessRet ProcessSignalCtrl::MaskReplace(ProcessSignalSet&& set)
{
    ProcessSignalSet old_set;
    return MaskReplace(set, old_set);
}

ProcessRet ProcessSignalCtrl::MaskReplace(ProcessSignalSet& new_set, ProcessSignalSet& old_set)
{
    return MaskReplace(std::move(new_set), std::move(old_set));
}

ProcessRet ProcessSignalCtrl::MaskReplace(ProcessSignalSet&& new_set, ProcessSignalSet&& old_set)
{
    ProcessRet ret = _mask_signal(SignalMaskType::REPLACE, std::move(new_set), std::move(old_set));
    if (ret != ProcessRet::SUCCESS) {
        return ret;
    }
    mask_set_ = new_set;
    last_mask_set_ = old_set;
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessSignalCtrl::UnMask()
{
    ProcessSignalSet new_set;
    ProcessSignalSet old_set;
    new_set.AddAll();
    return UnMask(new_set, old_set);
}

ProcessRet ProcessSignalCtrl::UnMask(ProcessSignalSet& set)
{
    return UnMask(std::move(set));
}

ProcessRet ProcessSignalCtrl::UnMask(ProcessSignalSet&& set)
{
    ProcessSignalSet old_set;
    return UnMask(set, old_set);
}

ProcessRet ProcessSignalCtrl::UnMask(ProcessSignalSet& new_set, ProcessSignalSet& old_set)
{
    return UnMask(std::move(new_set), std::move(old_set));
}

ProcessRet ProcessSignalCtrl::UnMask(ProcessSignalSet&& new_set, ProcessSignalSet&& old_set)
{
    ProcessRet ret = _mask_signal(SignalMaskType::SUBTRACT, std::move(new_set), std::move(old_set));
    if (ret != ProcessRet::SUCCESS) {
        return ret;
    }
    last_mask_set_ = old_set;
    ret = _mask_signal(SignalMaskType::GETMASK, std::move(mask_set_), std::move(mask_set_));
    if (ret != ProcessRet::SUCCESS) {
        return ret;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessSignalCtrl::MaskRevert()
{
    ProcessSignalSet set = last_mask_set_;
    return Mask(set);
}

ProcessRet ProcessSignalCtrl::_register_signal(ProcessSignal&& sig, ProcessSignalAction&& new_action, ProcessSignalAction&& old_action)
{
    if (sigaction(sig.sig_, &new_action.action_, &old_action.action_) < 0) {
        return ProcessRet::PROCESS_ESIGNALREG;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessSignalCtrl::_mask_signal(SignalMaskType how, ProcessSignalSet&& new_set, ProcessSignalSet&& old_set)
{
    switch (how) {
        case SignalMaskType::GETMASK:
            if (sigprocmask(SIG_BLOCK, NULL, &old_set.set_) < 0) {
                PROCESS_ERROR("Get process mask signal set error.");
                return ProcessRet::PROCESS_ESIGNALMASK;
            }
            break;
        case SignalMaskType::APPEND:
            if (sigprocmask(SIG_BLOCK, &new_set.set_, &old_set.set_) < 0) {
                PROCESS_ERROR("Set process mask signal set error.");
                return ProcessRet::PROCESS_ESIGNALMASK;
            }
            break;
        case SignalMaskType::SUBTRACT:
            if (sigprocmask(SIG_UNBLOCK, &new_set.set_, &old_set.set_) < 0) {
                PROCESS_ERROR("Set process mask signal set error.");
                return ProcessRet::PROCESS_ESIGNALMASK;
            }
            break;
        case SignalMaskType::REPLACE:
            if (sigprocmask(SIG_SETMASK, &new_set.set_, &old_set.set_) < 0) {
                PROCESS_ERROR("Set process mask signal set error.");
                return ProcessRet::PROCESS_ESIGNALMASK;
            }
            break;
        default:
            return ProcessRet::PROCESS_ESIGNALMASKTYPE;
    }
    return ProcessRet::SUCCESS;
}

}
