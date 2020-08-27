/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-18 04:15
 * @file     : process_signal.cc
 * @brief    : Linux signal Encapsulation.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#include "process_signal_id.h"

namespace infra::process {

SignalId::SignalId()
{

}

SignalId::SignalId(int sig) : sig_(sig)
{

}

SignalId::SignalId(const SignalId& other)
{
    sig_ = other.sig_;
}

SignalId::~SignalId()
{

}

SignalId& SignalId::operator=(const SignalId& other)
{
    sig_ = other.sig_;
    return *this;
}

bool SignalId::operator<(const SignalId& other) const
{
    return (sig_ < other.sig_);
}

}
