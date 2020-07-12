#include <stddef.h>

#include "thread_log.h"
#include "thread_signal_ctrl.h"

namespace thread::signal {

thread_local ThreadSignalCtrl* ThreadSignalCtrl::pInstance = NULL;

ThreadSignalCtrl::ThreadSignalCtrl()
{
    mempool_ = mempool::Mempool::getInstance();
}

ThreadSignalCtrl::~ThreadSignalCtrl()
{

}

ThreadSignalCtrl* ThreadSignalCtrl::getInstance()
{
    if (!pInstance) {
        pInstance = new ThreadSignalCtrl();
    }
    return pInstance;
}

process::signal::ProcessSignalSet ThreadSignalCtrl::GetSignalMask()
{
    return mask_set_;
}

ThreadRet ThreadSignalCtrl::Mask()
{
    process::signal::ProcessSignalSet new_set;
    process::signal::ProcessSignalSet old_set;
    new_set.AddAll();
    return Mask(new_set, old_set);
}

ThreadRet ThreadSignalCtrl::Mask(process::signal::ProcessSignalSet& set)
{
    return Mask(std::move(set));
}

ThreadRet ThreadSignalCtrl::Mask(process::signal::ProcessSignalSet&& set)
{
    process::signal::ProcessSignalSet old_set;
    return Mask(set, old_set);
}

ThreadRet ThreadSignalCtrl::Mask(process::signal::ProcessSignalSet& new_set, process::signal::ProcessSignalSet& old_set)
{
    return Mask(std::move(new_set), std::move(old_set));
}

ThreadRet ThreadSignalCtrl::Mask(process::signal::ProcessSignalSet&& new_set, process::signal::ProcessSignalSet&& old_set)
{
    ThreadRet ret = _mask_signal(process::signal::SignalMaskType::APPEND, std::move(new_set), std::move(old_set));
    if (ret != ThreadRet::SUCCESS) {
        return ret;
    }
    last_mask_set_ = old_set;
    ret = _mask_signal(process::signal::SignalMaskType::GETMASK, std::move(mask_set_), std::move(mask_set_));
    if (ret != ThreadRet::SUCCESS) {
        return ret;
    }
    return ThreadRet::SUCCESS;
}

ThreadRet ThreadSignalCtrl::MaskReplace(process::signal::ProcessSignalSet& set)
{
    return MaskReplace(std::move(set));
}

ThreadRet ThreadSignalCtrl::MaskReplace(process::signal::ProcessSignalSet&& set)
{
    process::signal::ProcessSignalSet old_set;
    return MaskReplace(set, old_set);
}

ThreadRet ThreadSignalCtrl::MaskReplace(process::signal::ProcessSignalSet& new_set, process::signal::ProcessSignalSet& old_set)
{
    return MaskReplace(std::move(new_set), std::move(old_set));
}

ThreadRet ThreadSignalCtrl::MaskReplace(process::signal::ProcessSignalSet&& new_set, process::signal::ProcessSignalSet&& old_set)
{
    ThreadRet ret = _mask_signal(process::signal::SignalMaskType::REPLACE, std::move(new_set), std::move(old_set));
    if (ret != ThreadRet::SUCCESS) {
        return ret;
    }
    mask_set_ = new_set;
    last_mask_set_ = old_set;
    return ThreadRet::SUCCESS;
}

ThreadRet ThreadSignalCtrl::UnMask()
{
    process::signal::ProcessSignalSet new_set;
    process::signal::ProcessSignalSet old_set;
    new_set.AddAll();
    return UnMask(new_set, old_set);
}

ThreadRet ThreadSignalCtrl::UnMask(process::signal::ProcessSignalSet& set)
{
    return UnMask(std::move(set));
}

ThreadRet ThreadSignalCtrl::UnMask(process::signal::ProcessSignalSet&& set)
{
    process::signal::ProcessSignalSet old_set;
    return UnMask(set, old_set);
}

ThreadRet ThreadSignalCtrl::UnMask(process::signal::ProcessSignalSet& new_set, process::signal::ProcessSignalSet& old_set)
{
    return UnMask(std::move(new_set), std::move(old_set));
}

ThreadRet ThreadSignalCtrl::UnMask(process::signal::ProcessSignalSet&& new_set, process::signal::ProcessSignalSet&& old_set)
{
    ThreadRet ret = _mask_signal(process::signal::SignalMaskType::SUBTRACT, std::move(new_set), std::move(old_set));
    if (ret != ThreadRet::SUCCESS) {
        return ret;
    }
    last_mask_set_ = old_set;
    ret = _mask_signal(process::signal::SignalMaskType::GETMASK, std::move(mask_set_), std::move(mask_set_));
    if (ret != ThreadRet::SUCCESS) {
        return ret;
    }
    return ThreadRet::SUCCESS;
}

ThreadRet ThreadSignalCtrl::MaskRevert()
{
    process::signal::ProcessSignalSet set = last_mask_set_;
    return Mask(set);
}

ThreadRet ThreadSignalCtrl::_mask_signal(process::signal::SignalMaskType how, process::signal::ProcessSignalSet&& new_set, process::signal::ProcessSignalSet&& old_set)
{
    switch (how) {
        case process::signal::SignalMaskType::GETMASK:
            if (pthread_sigmask(SIG_BLOCK, NULL, &old_set.set_) < 0) {
                THREAD_ERROR("Get process mask signal set error.");
                return ThreadRet::THREAD_ESIGNALMASK;
            }
            break;
        case process::signal::SignalMaskType::APPEND:
            if (pthread_sigmask(SIG_BLOCK, &new_set.set_, &old_set.set_) < 0) {
                THREAD_ERROR("Set process mask signal set error.");
                return ThreadRet::THREAD_ESIGNALMASK;
            }
            break;
        case process::signal::SignalMaskType::SUBTRACT:
            if (pthread_sigmask(SIG_UNBLOCK, &new_set.set_, &old_set.set_) < 0) {
                THREAD_ERROR("Set process mask signal set error.");
                return ThreadRet::THREAD_ESIGNALMASK;
            }
            break;
        case process::signal::SignalMaskType::REPLACE:
            if (pthread_sigmask(SIG_SETMASK, &new_set.set_, &old_set.set_) < 0) {
                THREAD_ERROR("Set process mask signal set error.");
                return ThreadRet::THREAD_ESIGNALMASK;
            }
            break;
        default:
            return ThreadRet::THREAD_ESIGNALMASKTYPE;
    }
    return ThreadRet::SUCCESS;
}

}
