#include "process_role.h"

namespace infra::process {

unsigned int Role::Normal = 0x01;

unsigned int Role::Parent = Role::Normal << 1;
unsigned int Role::Child = Role::Normal << 2;

unsigned int Role::PoolWorker = Role::Normal << 3;
unsigned int Role::PoolKeeper = Role::Normal << 4;

Role::Role()
{
    role_ = 0;
}

Role::Role(Role& other)
{
    role_ = other.role_;
}

Role::Role(unsigned int role)
{
    role_ = role;
}

Role::~Role()
{

}

Role& Role::Clean()
{
    role_ = 0;
    return *this;
}

Role Role::operator~()
{
    return Role(~role_);
}

Role& Role::operator=(Role& a)
{
    role_ = a.role_;
    return *this;
}
Role& Role::operator=(Role&& a)
{
    role_ = a.role_;
    return *this;
}

Role Role::operator|(Role& a)
{
    return Role(role_ | a.role_);
}
Role Role::operator|(Role&& a)
{
    return Role(role_ | a.role_);
}

Role& Role::operator|=(Role& a)
{
    role_ |= a.role_;
    return *this;
}
Role& Role::operator|=(Role&& a)
{
    role_ |= a.role_;
    return *this;
}

Role Role::operator&(Role& a)
{
    return Role(role_ & a.role_);
}
Role Role::operator&(Role&& a)
{
    return Role(role_ & a.role_);
}

Role& Role::operator&=(Role& a)
{
    role_ &= a.role_;
    return *this;
}
Role& Role::operator&=(Role&& a)
{
    role_ &= a.role_;
    return *this;
}

Role& Role::operator=(unsigned int& a)
{
    role_ = a;
    return *this;
}
Role& Role::operator=(unsigned int&& a)
{
    role_ = a;
    return *this;
}
Role Role::operator|(unsigned int& a)
{
    return Role(role_ | a);
}
Role Role::operator|(unsigned int&& a)
{
    return Role(role_ | a);
}
Role& Role::operator|=(unsigned int& a)
{
    role_ |= a;
    return *this;
}
Role& Role::operator|=(unsigned int&& a)
{
    role_ |= a;
    return *this;
}
Role Role::operator&(unsigned int& a)
{
    return Role(role_ & a);
}
Role Role::operator&(unsigned int&& a)
{
    return Role(role_ & a);
}
Role& Role::operator&=(unsigned int& a)
{
    role_ &= a;
    return *this;
}
Role& Role::operator&=(unsigned int&& a)
{
    role_ &= a;
    return *this;
}

Role& Role::AddRole(unsigned int role)
{
    role_ |= role;
    return *this;
}

bool Role::HasRole(unsigned int role)
{
    return (role_ & role) ? true : false;
}

}
