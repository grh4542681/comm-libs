#include "process_role.h"

namespace process {

unsigned int ProcessRole::Normal = 0x01;

unsigned int ProcessRole::Parent = ProcessRole::Normal << 1;
unsigned int ProcessRole::Child = ProcessRole::Normal << 2;

unsigned int ProcessRole::PoolWorker = ProcessRole::Normal << 3;
unsigned int ProcessRole::PoolKeeper = ProcessRole::Normal << 4;

ProcessRole::ProcessRole()
{
    role_ = 0;
}

ProcessRole::ProcessRole(ProcessRole& other)
{
    role_ = other.role_;
}

ProcessRole::ProcessRole(unsigned int role)
{
    role_ = role;
}

ProcessRole::~ProcessRole()
{

}

ProcessRole& ProcessRole::Clean()
{
    role_ = 0;
    return *this;
}

ProcessRole ProcessRole::operator~()
{
    return ProcessRole(~role_);
}

ProcessRole& ProcessRole::operator=(ProcessRole& a)
{
    role_ = a.role_;
    return *this;
}
ProcessRole& ProcessRole::operator=(ProcessRole&& a)
{
    role_ = a.role_;
    return *this;
}

ProcessRole ProcessRole::operator|(ProcessRole& a)
{
    return ProcessRole(role_ | a.role_);
}
ProcessRole ProcessRole::operator|(ProcessRole&& a)
{
    return ProcessRole(role_ | a.role_);
}

ProcessRole& ProcessRole::operator|=(ProcessRole& a)
{
    role_ |= a.role_;
    return *this;
}
ProcessRole& ProcessRole::operator|=(ProcessRole&& a)
{
    role_ |= a.role_;
    return *this;
}

ProcessRole ProcessRole::operator&(ProcessRole& a)
{
    return ProcessRole(role_ & a.role_);
}
ProcessRole ProcessRole::operator&(ProcessRole&& a)
{
    return ProcessRole(role_ & a.role_);
}

ProcessRole& ProcessRole::operator&=(ProcessRole& a)
{
    role_ &= a.role_;
    return *this;
}
ProcessRole& ProcessRole::operator&=(ProcessRole&& a)
{
    role_ &= a.role_;
    return *this;
}

ProcessRole& ProcessRole::operator=(unsigned int& a)
{
    role_ = a;
    return *this;
}
ProcessRole& ProcessRole::operator=(unsigned int&& a)
{
    role_ = a;
    return *this;
}
ProcessRole ProcessRole::operator|(unsigned int& a)
{
    return ProcessRole(role_ | a);
}
ProcessRole ProcessRole::operator|(unsigned int&& a)
{
    return ProcessRole(role_ | a);
}
ProcessRole& ProcessRole::operator|=(unsigned int& a)
{
    role_ |= a;
    return *this;
}
ProcessRole& ProcessRole::operator|=(unsigned int&& a)
{
    role_ |= a;
    return *this;
}
ProcessRole ProcessRole::operator&(unsigned int& a)
{
    return ProcessRole(role_ & a);
}
ProcessRole ProcessRole::operator&(unsigned int&& a)
{
    return ProcessRole(role_ & a);
}
ProcessRole& ProcessRole::operator&=(unsigned int& a)
{
    role_ &= a;
    return *this;
}
ProcessRole& ProcessRole::operator&=(unsigned int&& a)
{
    role_ &= a;
    return *this;
}

ProcessRole& ProcessRole::AddRole(unsigned int role)
{
    role_ |= role;
    return *this;
}

bool ProcessRole::HasRole(unsigned int role)
{
    return (role_ & role) ? true : false;
}

}
