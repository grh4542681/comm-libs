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

namespace infra::process {

SignalCtrl* SignalCtrl::pInstance = NULL;

SignalCtrl::SignalCtrl()
{

}

SignalCtrl::~SignalCtrl()
{

}

SignalCtrl* SignalCtrl::getInstance()
{
    if (!pInstance) {
        pInstance = new SignalCtrl();
    }
    return pInstance;
}

SignalAction SignalCtrl::GetSignalAction(Signal& sig)
{
    return GetSignalAction(std::move(sig));
}

SignalAction SignalCtrl::GetSignalAction(Signal&& sig)
{
    SignalAction action;
    auto it = register_map_.find(sig);
    if (it != register_map_.end()) {
        action = it->second;
    }
    return action;
}

SignalSet SignalCtrl::GetSignalMask()
{
    return mask_set_;
}

Return SignalCtrl::Register(Signal& sig, SignalAction& action)
{
    return Register(std::move(sig), std::move(action));
}

Return SignalCtrl::Register(Signal&& sig, SignalAction&& action)
{
    SignalAction old_action;
    return Register(std::move(sig), std::move(action), std::move(old_action));
}

Return SignalCtrl::Register(Signal& sig, SignalAction& new_action, SignalAction& old_action)
{
    return Register(std::move(sig), std::move(new_action), std::move(old_action));
}

Return SignalCtrl::Register(Signal&& sig, SignalAction&& new_action, SignalAction&& old_action)
{
    Return ret = Return::SUCCESS;
    if ((ret = _register_signal(std::move(sig), std::move(new_action), std::move(old_action))) != Return::SUCCESS) {
        return ret;
    }
    //std::pair<std::map<Signal, SignalAction>::iterator, bool> ret;
    register_map_.insert_or_assign(sig, new_action);
    last_register_map_.insert_or_assign(sig, old_action);
    return Return::SUCCESS;
}

Return SignalCtrl::UnRegister()
{
    Return ret;
    Signal sig;
    std::map<Signal, SignalAction> register_map = register_map_;
    for (const auto& it : register_map) {
        sig = it.first;
        ret = UnRegister(std::move(sig));
        if ( ret != Return::SUCCESS) {
            return ret;
        }
    }
    return Return::SUCCESS;
}

Return SignalCtrl::UnRegister(Signal& sig)
{
    return UnRegister(std::move(sig));
}

Return SignalCtrl::UnRegister(Signal&& sig)
{
    SignalAction old_action;
    return UnRegister(std::move(sig), std::move(old_action));
}

Return SignalCtrl::UnRegister(Signal& sig, SignalAction& old_action)
{
    return UnRegister(std::move(sig), std::move(old_action));
}

Return SignalCtrl::UnRegister(Signal&& sig, SignalAction&& old_action)
{
    SignalAction default_action;
    return Register(std::move(sig), std::move(default_action), std::move(old_action));
}

Return SignalCtrl::Revert()
{
    Return ret;
    Signal sig;
    SignalAction action;
    std::map<Signal, SignalAction> register_map = last_register_map_;
    for (const auto& it : register_map) {
        sig = it.first;
        action = it.second;
        ret = Register(std::move(sig), std::move(action));
        if ( ret != Return::SUCCESS) {
            return ret;
        }
    }
    return Return::SUCCESS;
}

Return SignalCtrl::Revert(Signal& sig)
{
    return Revert(std::move(sig));
}

Return SignalCtrl::Revert(Signal&& sig)
{
    auto it = last_register_map_.find(sig);
    if (it == last_register_map_.end()) {
        return Return::PROCESS_ESIGNALNOTFOUND;
    }
    SignalAction action = it->second;
    return Register(std::move(sig), std::move(action));
}

Return SignalCtrl::Mask()
{
    SignalSet new_set;
    SignalSet old_set;
    new_set.AddAll();
    return Mask(new_set, old_set);
}

Return SignalCtrl::Mask(SignalSet& set)
{
    return Mask(std::move(set));
}

Return SignalCtrl::Mask(SignalSet&& set)
{
    SignalSet old_set;
    return Mask(set, old_set);
}

Return SignalCtrl::Mask(SignalSet& new_set, SignalSet& old_set)
{
    return Mask(std::move(new_set), std::move(old_set));
}

Return SignalCtrl::Mask(SignalSet&& new_set, SignalSet&& old_set)
{
    Return ret = _mask_signal(SignalMaskType::APPEND, std::move(new_set), std::move(old_set));
    if (ret != Return::SUCCESS) {
        return ret;
    }
    last_mask_set_ = old_set;
    ret = _mask_signal(SignalMaskType::GETMASK, std::move(mask_set_), std::move(mask_set_));
    if (ret != Return::SUCCESS) {
        return ret;
    }
    return Return::SUCCESS;
}

Return SignalCtrl::MaskReplace(SignalSet& set)
{
    return MaskReplace(std::move(set));
}

Return SignalCtrl::MaskReplace(SignalSet&& set)
{
    SignalSet old_set;
    return MaskReplace(set, old_set);
}

Return SignalCtrl::MaskReplace(SignalSet& new_set, SignalSet& old_set)
{
    return MaskReplace(std::move(new_set), std::move(old_set));
}

Return SignalCtrl::MaskReplace(SignalSet&& new_set, SignalSet&& old_set)
{
    Return ret = _mask_signal(SignalMaskType::REPLACE, std::move(new_set), std::move(old_set));
    if (ret != Return::SUCCESS) {
        return ret;
    }
    mask_set_ = new_set;
    last_mask_set_ = old_set;
    return Return::SUCCESS;
}

Return SignalCtrl::UnMask()
{
    SignalSet new_set;
    SignalSet old_set;
    new_set.AddAll();
    return UnMask(new_set, old_set);
}

Return SignalCtrl::UnMask(SignalSet& set)
{
    return UnMask(std::move(set));
}

Return SignalCtrl::UnMask(SignalSet&& set)
{
    SignalSet old_set;
    return UnMask(set, old_set);
}

Return SignalCtrl::UnMask(SignalSet& new_set, SignalSet& old_set)
{
    return UnMask(std::move(new_set), std::move(old_set));
}

Return SignalCtrl::UnMask(SignalSet&& new_set, SignalSet&& old_set)
{
    Return ret = _mask_signal(SignalMaskType::SUBTRACT, std::move(new_set), std::move(old_set));
    if (ret != Return::SUCCESS) {
        return ret;
    }
    last_mask_set_ = old_set;
    ret = _mask_signal(SignalMaskType::GETMASK, std::move(mask_set_), std::move(mask_set_));
    if (ret != Return::SUCCESS) {
        return ret;
    }
    return Return::SUCCESS;
}

Return SignalCtrl::MaskRevert()
{
    SignalSet set = last_mask_set_;
    return Mask(set);
}

Return SignalCtrl::_register_signal(Signal&& sig, SignalAction&& new_action, SignalAction&& old_action)
{
    if (sigaction(sig.sig_, &new_action.action_, &old_action.action_) < 0) {
        return Return::PROCESS_ESIGNALREG;
    }
    return Return::SUCCESS;
}

Return SignalCtrl::_mask_signal(SignalMaskType how, SignalSet&& new_set, SignalSet&& old_set)
{
    switch (how) {
        case SignalMaskType::GETMASK:
            if (sigprocmask(SIG_BLOCK, NULL, &old_set.set_) < 0) {
                Log::Error("Get process mask signal set error.");
                return Return::PROCESS_ESIGNALMASK;
            }
            break;
        case SignalMaskType::APPEND:
            if (sigprocmask(SIG_BLOCK, &new_set.set_, &old_set.set_) < 0) {
                Log::Error("Set process mask signal set error.");
                return Return::PROCESS_ESIGNALMASK;
            }
            break;
        case SignalMaskType::SUBTRACT:
            if (sigprocmask(SIG_UNBLOCK, &new_set.set_, &old_set.set_) < 0) {
                Log::Error("Set process mask signal set error.");
                return Return::PROCESS_ESIGNALMASK;
            }
            break;
        case SignalMaskType::REPLACE:
            if (sigprocmask(SIG_SETMASK, &new_set.set_, &old_set.set_) < 0) {
                Log::Error("Set process mask signal set error.");
                return Return::PROCESS_ESIGNALMASK;
            }
            break;
        default:
            return Return::PROCESS_ESIGNALMASKTYPE;
    }
    return Return::SUCCESS;
}

}
