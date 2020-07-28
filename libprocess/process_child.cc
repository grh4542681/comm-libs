#include "process_child.h"

namespace infra::process {

Child::Child()
{
    name_.clear();
    role_ |= Role::Child;
    state_ = State::Normal;

    dead_callback_ = NULL;
}

Child::Child(std::string name, ID&& pid)
{
    name_ = name;
    pid_ = pid;
    role_ |= Role::Child;
    state_ = State::Normal;

    dead_callback_ = NULL;
}

Child::Child(Child& other)
{
    pid_ = other.pid_;
    name_ = other.name_;
    role_ = other.role_;
    state_ = other.state_;

    dead_callback_ = other.dead_callback_;
    fd_ = other.fd_;
}

Child::~Child()
{

}

ID& Child::GetPid()
{
    return pid_;
}

std::string Child::GetName()
{
    return name_;
}

Role& Child::GetRole()
{
    return role_;
}

State& Child::GetState()
{
    return state_;
}

Child::ChildDeadCallback_t Child::GetDeadCallback()
{
    return dead_callback_;
}

Child& Child::AddRole(Role&& role)
{
    role_ |= role;
    return *this;
}

Child& Child::DelRole(Role&& role)
{
    role_ &= ~role;
    return *this;
}

Child& Child::SetState(State&& state)
{
    state_ = state;
    return *this;
}

Child& Child::SetDeadCallback(ChildDeadCallback_t& dead_callback)
{
    dead_callback_ = dead_callback;
    return *this;
}

ssize_t Child::Write(const void* data, size_t datalen)
{
    return 0;
}

ssize_t Child::Read(void* data, size_t datalen)
{
    return 0;
}

}
