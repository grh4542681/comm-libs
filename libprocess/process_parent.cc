#include "process_parent.h"

namespace infra::process {

Parent::Parent() : io::FD()
{
    name_.clear();
}

Parent::Parent(std::string name, ID& pid) : io::FD()
{
    name_ = name;
    pid_ = pid;
}

Parent::Parent(std::string name, ID&& pid) : io::FD()
{
    name_ = name;
    pid_ = pid;
}

Parent::Parent(Parent& other) : io::FD(other)
{
    pid_ = other.pid_;
    name_ = other.name_;
}

Parent::~Parent()
{

}

ID& Parent::GetPid()
{
    return pid_;
}

std::string Parent::GetName()
{
    return name_;
}

Role Parent::GetRole()
{
    return role_;
}

State Parent::GetState()
{
    return state_;
}

Parent& Parent::SetRole(Role&& role)
{
    role_ = role;
    return *this;
}

Parent& Parent::SetState(State&& state)
{
    state_ = state;
    return *this;
}

ssize_t Parent::Write(const void* data, size_t datalen)
{
    return 0;
}

ssize_t Parent::Read(void* data, size_t datalen)
{
    return 0;
}

}
