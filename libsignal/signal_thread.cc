#include <stddef.h>

#include "signal_log.h"
#include "signal_thread.h"

namespace infra::signal {

thread_local Thread* Thread::pInstance = NULL;

Thread::Thread()
{
    
}

Thread::~Thread()
{

}

Thread* Thread::Instance()
{
    if (!pInstance) {
        pInstance = new Thread();
    }
    return pInstance;
}

Set Thread::GetSignalMask()
{
    return mask_set_;
}

Return Thread::Mask()
{
    Set new_set;
    Set old_set;
    new_set.AddAll();
    return Mask(new_set, old_set);
}

Return Thread::Mask(Set& set)
{
    return Mask(std::move(set));
}

Return Thread::Mask(Set&& set)
{
    Set old_set;
    return Mask(set, old_set);
}

Return Thread::Mask(Set& new_set, Set& old_set)
{
    return Mask(std::move(new_set), std::move(old_set));
}

Return Thread::Mask(Set&& new_set, Set&& old_set)
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

Return Thread::MaskReplace(Set& set)
{
    return MaskReplace(std::move(set));
}

Return Thread::MaskReplace(Set&& set)
{
    Set old_set;
    return MaskReplace(set, old_set);
}

Return Thread::MaskReplace(Set& new_set, Set& old_set)
{
    return MaskReplace(std::move(new_set), std::move(old_set));
}

Return Thread::MaskReplace(Set&& new_set, Set&& old_set)
{
    Return ret = _mask_signal(SignalMaskType::REPLACE, std::move(new_set), std::move(old_set));
    if (ret != Return::SUCCESS) {
        return ret;
    }
    mask_set_ = new_set;
    last_mask_set_ = old_set;
    return Return::SUCCESS;
}

Return Thread::UnMask()
{
    Set new_set;
    Set old_set;
    new_set.AddAll();
    return UnMask(new_set, old_set);
}

Return Thread::UnMask(Set& set)
{
    return UnMask(std::move(set));
}

Return Thread::UnMask(Set&& set)
{
    Set old_set;
    return UnMask(set, old_set);
}

Return Thread::UnMask(Set& new_set, Set& old_set)
{
    return UnMask(std::move(new_set), std::move(old_set));
}

Return Thread::UnMask(Set&& new_set, Set&& old_set)
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

Return Thread::MaskRevert()
{
    Set set = last_mask_set_;
    return Mask(set);
}

Return Thread::_mask_signal(SignalMaskType how, Set&& new_set, Set&& old_set)
{
    switch (how) {
        case SignalMaskType::GETMASK:
            if (pthread_sigmask(SIG_BLOCK, NULL, &old_set.set_) < 0) {
                Log::Error("Get process mask signal set error.");
                return Return::SIGNAL_EMASK;
            }
            break;
        case SignalMaskType::APPEND:
            if (pthread_sigmask(SIG_BLOCK, &new_set.set_, &old_set.set_) < 0) {
                Log::Error("Set process mask signal set error.");
                return Return::SIGNAL_EMASK;
            }
            break;
        case SignalMaskType::SUBTRACT:
            if (pthread_sigmask(SIG_UNBLOCK, &new_set.set_, &old_set.set_) < 0) {
                Log::Error("Set process mask signal set error.");
                return Return::SIGNAL_EMASK;
            }
            break;
        case SignalMaskType::REPLACE:
            if (pthread_sigmask(SIG_SETMASK, &new_set.set_, &old_set.set_) < 0) {
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
