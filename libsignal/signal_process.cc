#include <stddef.h>

#include "signal_log.h"
#include "signal_process.h"

namespace infra::signal {

Process* Process::pInstance = NULL;

Process::Process()
{

}

Process::~Process()
{

}

Process& Process::Instance()
{
    if (!pInstance) {
        pInstance = new Process();
    }
    return *pInstance;
}

Action Process::GetAction(ID& sig)
{
    return GetAction(std::move(sig));
}

Action Process::GetAction(ID&& sig)
{
    Action action;
    auto it = register_map_.find(sig);
    if (it != register_map_.end()) {
        action = it->second;
    }
    return action;
}

Set Process::GetSignalMask()
{
    return mask_set_;
}

Return Process::Register(ID& sig, Action& action)
{
    return Register(std::move(sig), std::move(action));
}

Return Process::Register(ID&& sig, Action&& action)
{
    Action old_action;
    return Register(std::move(sig), std::move(action), std::move(old_action));
}

Return Process::Register(ID& sig, Action& new_action, Action& old_action)
{
    return Register(std::move(sig), std::move(new_action), std::move(old_action));
}

Return Process::Register(ID&& sig, Action&& new_action, Action&& old_action)
{
    Return ret = Return::SUCCESS;
    if ((ret = _register_signal(std::move(sig), std::move(new_action), std::move(old_action))) != Return::SUCCESS) {
        return ret;
    }
    //std::pair<std::map<Signal, Action>::iterator, bool> ret;
    register_map_.insert_or_assign(sig, new_action);
    last_register_map_.insert_or_assign(sig, old_action);
    return Return::SUCCESS;
}

Return Process::UnRegister()
{
    Return ret;
    ID sig;
    std::map<ID, Action> register_map = register_map_;
    for (const auto& it : register_map) {
        sig = it.first;
        ret = UnRegister(std::move(sig));
        if ( ret != Return::SUCCESS) {
            return ret;
        }
    }
    return Return::SUCCESS;
}

Return Process::UnRegister(ID& sig)
{
    return UnRegister(std::move(sig));
}

Return Process::UnRegister(ID&& sig)
{
    Action old_action;
    return UnRegister(std::move(sig), std::move(old_action));
}

Return Process::UnRegister(ID& sig, Action& old_action)
{
    return UnRegister(std::move(sig), std::move(old_action));
}

Return Process::UnRegister(ID&& sig, Action&& old_action)
{
    Action default_action;
    return Register(std::move(sig), std::move(default_action), std::move(old_action));
}

Return Process::Revert()
{
    Return ret;
    ID sig;
    Action action;
    std::map<ID, Action> register_map = last_register_map_;
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

Return Process::Revert(ID& sig)
{
    return Revert(std::move(sig));
}

Return Process::Revert(ID&& sig)
{
    auto it = last_register_map_.find(sig);
    if (it == last_register_map_.end()) {
        return Return::SIGNAL_ENOTFOUND;
    }
    Action action = it->second;
    return Register(std::move(sig), std::move(action));
}

Return Process::Mask()
{
    Set new_set;
    Set old_set;
    new_set.AddAll();
    return Mask(new_set, old_set);
}

Return Process::Mask(Set& set)
{
    return Mask(std::move(set));
}

Return Process::Mask(Set&& set)
{
    Set old_set;
    return Mask(set, old_set);
}

Return Process::Mask(Set& new_set, Set& old_set)
{
    return Mask(std::move(new_set), std::move(old_set));
}

Return Process::Mask(Set&& new_set, Set&& old_set)
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

Return Process::MaskReplace(Set& set)
{
    return MaskReplace(std::move(set));
}

Return Process::MaskReplace(Set&& set)
{
    Set old_set;
    return MaskReplace(set, old_set);
}

Return Process::MaskReplace(Set& new_set, Set& old_set)
{
    return MaskReplace(std::move(new_set), std::move(old_set));
}

Return Process::MaskReplace(Set&& new_set, Set&& old_set)
{
    Return ret = _mask_signal(SignalMaskType::REPLACE, std::move(new_set), std::move(old_set));
    if (ret != Return::SUCCESS) {
        return ret;
    }
    mask_set_ = new_set;
    last_mask_set_ = old_set;
    return Return::SUCCESS;
}

Return Process::UnMask()
{
    Set new_set;
    Set old_set;
    new_set.AddAll();
    return UnMask(new_set, old_set);
}

Return Process::UnMask(Set& set)
{
    return UnMask(std::move(set));
}

Return Process::UnMask(Set&& set)
{
    Set old_set;
    return UnMask(set, old_set);
}

Return Process::UnMask(Set& new_set, Set& old_set)
{
    return UnMask(std::move(new_set), std::move(old_set));
}

Return Process::UnMask(Set&& new_set, Set&& old_set)
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

Return Process::MaskRevert()
{
    Set set = last_mask_set_;
    return Mask(set);
}

Return Process::_register_signal(ID&& sig, Action&& new_action, Action&& old_action)
{
    if (sigaction(sig.sig_, &new_action.action_, &old_action.action_) < 0) {
        return Return::SIGNAL_EREGISTER;
    }
    return Return::SUCCESS;
}

Return Process::_mask_signal(SignalMaskType how, Set&& new_set, Set&& old_set)
{
    switch (how) {
        case SignalMaskType::GETMASK:
            if (sigprocmask(SIG_BLOCK, NULL, &old_set.set_) < 0) {
                Log::Error("Get process mask signal set error.");
                return Return::SIGNAL_EMASK;
            }
            break;
        case SignalMaskType::APPEND:
            if (sigprocmask(SIG_BLOCK, &new_set.set_, &old_set.set_) < 0) {
                Log::Error("Set process mask signal set error.");
                return Return::SIGNAL_EMASK;
            }
            break;
        case SignalMaskType::SUBTRACT:
            if (sigprocmask(SIG_UNBLOCK, &new_set.set_, &old_set.set_) < 0) {
                Log::Error("Set process mask signal set error.");
                return Return::SIGNAL_EMASK;
            }
            break;
        case SignalMaskType::REPLACE:
            if (sigprocmask(SIG_SETMASK, &new_set.set_, &old_set.set_) < 0) {
                Log::Error("Set process mask signal set error.");
                return Return::SIGNAL_EMASK;
            }
            break;
        default:
            return Return::SIGNAL_EMASKTYPE;
    }
    return Return::SUCCESS;
}

}
