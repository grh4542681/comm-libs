#include "signal_set.h"

namespace infra::signal {

Set::Set()
{
    sigemptyset(&set_);
    empty_flag_ = true;
}

Set::Set(const Set& other)
{
    set_ = other.set_;
    empty_flag_ = other.empty_flag_;
}

Set::~Set()
{
    sigemptyset(&set_);
}

const Set& Set::operator=(const Set& other)
{
    set_ = other.set_;
    empty_flag_ = other.empty_flag_;
    return *this;
}

bool Set::IsEmpty()
{
    return empty_flag_;
}

Return Set::AddSig(ID& sig)
{
    return AddSig(std::move(sig));
}
Return Set::AddSig(ID&& sig)
{
    if (!sigaddset(&set_, sig.sig_)) {
        return Return::SIGNAL_EINVAL;
    }
    empty_flag_ = false;
    return Return::SUCCESS;
}

Return Set::AddAll()
{
    if (!sigfillset(&set_)) {
        return Return::SIGNAL_EINVAL;
    }
    empty_flag_ = false;
    return Return::SUCCESS;
}

Return Set::DelSig(ID& sig)
{
    return DelSig(std::move(sig));
}
Return Set::DelSig(ID&& sig)
{
    if (!sigdelset(&set_, sig.sig_)) {
        return Return::SIGNAL_EINVAL;
    }
    empty_flag_ = false;
    return Return::SUCCESS;
}


Return Set::DelAll()
{
    if (!sigemptyset(&set_)) {
        return Return::SIGNAL_EINVAL;
    }
    empty_flag_ = true;
    return Return::SUCCESS;
}

bool Set::HasSig(ID& sig)
{
    if (sigismember(&set_, sig.sig_) == 1) {
        return true;
    }
    return false;
}

sigset_t* Set::GetSigset()
{
    return &set_;
}

}
