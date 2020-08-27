/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-18 04:06
 * @file     : process_signal_action.cc
 * @brief    : Signal callback action.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#include "string.h"
#include "stddef.h"

#include "process_signal_action.h"

namespace infra::process {

Action::Action()
{
    memset(&action_, 0, sizeof(struct sigaction));
     action_.sa_handler = SIG_DFL;
}

Action::Action(struct sigaction* action)
{
    memset(&action_, 0, sizeof(struct sigaction));
    action_.sa_handler = action->sa_handler;
    action_.sa_sigaction = action->sa_sigaction;
    action_.sa_mask = action->sa_mask;
    action_.sa_flags = action->sa_flags;
}

Action::Action(const Action& other)
{
    memset(&action_, 0, sizeof(struct sigaction));
    action_.sa_handler = other.action_.sa_handler;
    action_.sa_sigaction = other.action_.sa_sigaction;
    action_.sa_mask = other.action_.sa_mask;
    action_.sa_flags = other.action_.sa_flags;
}

Action::~Action()
{

}

Action& Action::operator=(const Action& other)
{
    action_.sa_handler = other.action_.sa_handler;
    action_.sa_sigaction = other.action_.sa_sigaction;
    action_.sa_mask = other.action_.sa_mask;
    action_.sa_flags = other.action_.sa_flags;
    return *this;
}

bool Action::IsDefault()
{
    return (action_.sa_handler == SIG_DFL);
}

Action& Action::SetCallback(SignalCallback callback)
{
    action_.sa_handler = callback;
    action_.sa_flags &= ~SA_SIGINFO;
    return *this;
}

Action& Action::SetCallback(SignalCallback2 callback)
{
    action_.sa_sigaction = callback;
    action_.sa_flags |= SA_SIGINFO;
    return *this;
}

Action& Action::SetMaskset(Set& set)
{
    return SetMaskset(std::move(set));
}
Action& Action::SetMaskset(Set&& set)
{
    action_.sa_mask = set.set_;
    return *this;
}

}
