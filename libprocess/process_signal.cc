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

namespace infra::process {

Signal::Signal()
{

}

Signal::Signal(int sig) : sig_(sig)
{

}

Signal::Signal(const Signal& other)
{
    sig_ = other.sig_;
}

Signal::~Signal()
{

}

Signal& Signal::operator=(const Signal& other)
{
    sig_ = other.sig_;
    return *this;
}

bool Signal::operator<(const Signal& other) const
{
    return (sig_ < other.sig_);
}

}
