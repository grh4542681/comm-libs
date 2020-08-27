#include <stddef.h>

#include "process_log.h"
#include "process_signal.h"

namespace infra::process {

Signal* Signal::pInstance = NULL;

Signal::Signal()
{

}

Signal::~Signal()
{

}

Signal& Signal::Instance()
{
    if (!pInstance) {
        pInstance = new Signal();
    }
    return *pInstance;
}

SignalAction Signal::GetSignalAction(SignalId& sig)
{
    return GetSignalAction(std::move(sig));
}

SignalAction Signal::GetSignalAction(SignalId&& sig)
{
    SignalAction action;
    auto it = register_map_.find(sig);
    if (it != register_map_.end()) {
        action = it->second;
    }
    return action;
}

SignalSet Signal::GetSignalMask()
{
    return mask_set_;
}

Return Signal::Register(SignalId& sig, SignalAction& action)
{
    return Register(std::move(sig), std::move(action));
}

Return Signal::Register(SignalId&& sig, SignalAction&& action)
{
    SignalAction old_action;
    return Register(std::move(sig), std::move(action), std::move(old_action));
}

Return Signal::Register(SignalId& sig, SignalAction& new_action, SignalAction& old_action)
{
    return Register(std::move(sig), std::move(new_action), std::move(old_action));
}

Return Signal::Register(SignalId&& sig, SignalAction&& new_action, SignalAction&& old_action)
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

Return Signal::UnRegister()
{
    Return ret;
    SignalId sig;
    std::map<SignalId, SignalAction> register_map = register_map_;
    for (const auto& it : register_map) {
        sig = it.first;
        ret = UnRegister(std::move(sig));
        if ( ret != Return::SUCCESS) {
            return ret;
        }
    }
    return Return::SUCCESS;
}

Return Signal::UnRegister(SignalId& sig)
{
    return UnRegister(std::move(sig));
}

Return Signal::UnRegister(SignalId&& sig)
{
    SignalAction old_action;
    return UnRegister(std::move(sig), std::move(old_action));
}

Return Signal::UnRegister(SignalId& sig, SignalAction& old_action)
{
    return UnRegister(std::move(sig), std::move(old_action));
}

Return Signal::UnRegister(SignalId&& sig, SignalAction&& old_action)
{
    SignalAction default_action;
    return Register(std::move(sig), std::move(default_action), std::move(old_action));
}

Return Signal::Revert()
{
    Return ret;
    SignalId sig;
    SignalAction action;
    std::map<SignalId, SignalAction> register_map = last_register_map_;
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

Return Signal::Revert(SignalId& sig)
{
    return Revert(std::move(sig));
}

Return Signal::Revert(SignalId&& sig)
{
    auto it = last_register_map_.find(sig);
    if (it == last_register_map_.end()) {
        return Return::PROCESS_ESIGNALNOTFOUND;
    }
    SignalAction action = it->second;
    return Register(std::move(sig), std::move(action));
}

Return Signal::Mask()
{
    SignalSet new_set;
    SignalSet old_set;
    new_set.AddAll();
    return Mask(new_set, old_set);
}

Return Signal::Mask(SignalSet& set)
{
    return Mask(std::move(set));
}

Return Signal::Mask(SignalSet&& set)
{
    SignalSet old_set;
    return Mask(set, old_set);
}

Return Signal::Mask(SignalSet& new_set, SignalSet& old_set)
{
    return Mask(std::move(new_set), std::move(old_set));
}

Return Signal::Mask(SignalSet&& new_set, SignalSet&& old_set)
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

Return Signal::MaskReplace(SignalSet& set)
{
    return MaskReplace(std::move(set));
}

Return Signal::MaskReplace(SignalSet&& set)
{
    SignalSet old_set;
    return MaskReplace(set, old_set);
}

Return Signal::MaskReplace(SignalSet& new_set, SignalSet& old_set)
{
    return MaskReplace(std::move(new_set), std::move(old_set));
}

Return Signal::MaskReplace(SignalSet&& new_set, SignalSet&& old_set)
{
    Return ret = _mask_signal(SignalMaskType::REPLACE, std::move(new_set), std::move(old_set));
    if (ret != Return::SUCCESS) {
        return ret;
    }
    mask_set_ = new_set;
    last_mask_set_ = old_set;
    return Return::SUCCESS;
}

Return Signal::UnMask()
{
    SignalSet new_set;
    SignalSet old_set;
    new_set.AddAll();
    return UnMask(new_set, old_set);
}

Return Signal::UnMask(SignalSet& set)
{
    return UnMask(std::move(set));
}

Return Signal::UnMask(SignalSet&& set)
{
    SignalSet old_set;
    return UnMask(set, old_set);
}

Return Signal::UnMask(SignalSet& new_set, SignalSet& old_set)
{
    return UnMask(std::move(new_set), std::move(old_set));
}

Return Signal::UnMask(SignalSet&& new_set, SignalSet&& old_set)
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

Return Signal::MaskRevert()
{
    SignalSet set = last_mask_set_;
    return Mask(set);
}

Return Signal::_register_signal(SignalId&& sig, SignalAction&& new_action, SignalAction&& old_action)
{
    if (sigaction(sig.sig_, &new_action.action_, &old_action.action_) < 0) {
        return Return::PROCESS_ESIGNALREG;
    }
    return Return::SUCCESS;
}

Return Signal::_mask_signal(SignalMaskType how, SignalSet&& new_set, SignalSet&& old_set)
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
