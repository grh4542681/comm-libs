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
#include "process_signal.h"

namespace infra::process::signal {

ProcessSignal::ProcessSignal()
{

}

ProcessSignal::ProcessSignal(int sig) : sig_(sig)
{

}

ProcessSignal::ProcessSignal(const ProcessSignal& other)
{
    sig_ = other.sig_;
}

ProcessSignal::~ProcessSignal()
{

}

ProcessSignal& ProcessSignal::operator=(const ProcessSignal& other)
{
    sig_ = other.sig_;
    return *this;
}

bool ProcessSignal::operator<(const ProcessSignal& other) const
{
    return (sig_ < other.sig_);
}

}
