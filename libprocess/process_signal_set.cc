/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-18 04:14
 * @file     : process_signal_set.cc
 * @brief    : Signal set.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#include "process_signal_set.h"

namespace infra::process {

SignalSet::SignalSet()
{
    sigemptyset(&set_);
    empty_flag_ = true;
}

SignalSet::SignalSet(const SignalSet& other)
{
    set_ = other.set_;
    empty_flag_ = other.empty_flag_;
}

SignalSet::~SignalSet()
{
    sigemptyset(&set_);
}

const SignalSet& SignalSet::operator=(const SignalSet& other)
{
    set_ = other.set_;
    empty_flag_ = other.empty_flag_;
    return *this;
}

bool SignalSet::IsEmpty()
{
    return empty_flag_;
}

Return SignalSet::AddSig(Signal& sig)
{
    return AddSig(std::move(sig));
}
Return SignalSet::AddSig(Signal&& sig)
{
    if (!sigaddset(&set_, sig.sig_)) {
        return Return::PROCESS_ESIGINVAL;
    }
    empty_flag_ = false;
    return Return::SUCCESS;
}

Return SignalSet::AddAll()
{
    if (!sigfillset(&set_)) {
        return Return::PROCESS_ESIGINVAL;
    }
    empty_flag_ = false;
    return Return::SUCCESS;
}

Return SignalSet::DelSig(Signal& sig)
{
    return DelSig(std::move(sig));
}
Return SignalSet::DelSig(Signal&& sig)
{
    if (!sigdelset(&set_, sig.sig_)) {
        return Return::PROCESS_ESIGINVAL;
    }
    empty_flag_ = false;
    return Return::SUCCESS;
}


Return SignalSet::DelAll()
{
    if (!sigemptyset(&set_)) {
        return Return::PROCESS_ESIGINVAL;
    }
    empty_flag_ = true;
    return Return::SUCCESS;
}

bool SignalSet::HasSig(Signal& sig)
{
    if (sigismember(&set_, sig.sig_) == 1) {
        return true;
    }
    return false;
}

sigset_t* SignalSet::GetSigset()
{
    return &set_;
}

}
