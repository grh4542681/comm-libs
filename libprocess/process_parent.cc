#include "process_parent.h"

namespace infra::process {

Parent::Parent() : io::FD()
{
    name_.clear();
    role_ += Role::Type::Parent;
    relation_ += Relation::Type::Parent;
    state_ = State::Normal;
}

Parent::Parent(std::string name, ID& pid) : io::FD()
{
    name_ = name;
    pid_ = pid;
    role_ += Role::Type::Parent;
    relation_ += Relation::Type::Parent;
    state_ = State::Normal;
}

Parent::Parent(std::string name, ID&& pid) : io::FD()
{
    name_ = name;
    pid_ = pid;
    role_ += Role::Type::Parent;
    relation_ += Relation::Type::Parent;
    state_ = State::Normal;
}

Parent::Parent(const Parent& other) : io::FD(other)
{
    pid_ = other.pid_;
    name_ = other.name_;
    role_ = other.role_;
    relation_ = other.relation_;
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

Parent& Parent::SetState(State&& state)
{
    state_ = state;
    return *this;
}

Parent& Parent::AddRole(Role&& role)
{
    role_ |= role;
    return *this;
}

Parent& Parent::DelRole(Role&& role)
{
    role_ &= ~role;
    return *this;
}

Parent& Parent::AddRelation(Relation&& relation)
{
    relation_ += relation;
    return *this;
}

Parent& Parent::DelRelation(Relation&& relation)
{
    relation_ -= relation;
    return *this;
}

io::FD* Parent::Clone()
{
    return nullptr;
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
