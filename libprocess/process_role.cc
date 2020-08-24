#include "process_role.h"

namespace infra::process {

std::vector<std::tuple<Role::Type, std::string, std::string>> Role::TypeDescription = {
    //   type        string          describe
    { Type::Normal, "Normal", "Default role" },
    { Type::Parent, "Parent", "Parent process" },
    { Type::Child, "Child", "Child process" },
    { Type::PoolWorker, "PoolWorker", "Process pool worker" },
    { Type::PoolKeeper, "PoolKeeper", "Process pool manager" },
};

Role::Role() { }
Role::Role(Role::Type&& type)
{
    _BitSet(std::forward<Role::Type>(type));
}
Role::Role(std::vector<Role::Type>&& roles)
{
    for(auto type : roles) {
        _BitSet(std::move(type));
    }
}
Role::Role(const Role& other)
{
    role_ = other.role_;
}
Role::Role(const Role&& other)
{
    role_ = other.role_;
}
Role::~Role() { }

Role& Role::operator=(const Role& other)
{
    role_ = other.role_;
    return *this;
}

Role Role::operator+(Role& role)
{
    Role other(*this);
    other._BitSet(std::move(role.role_));
    return other;
}
Role Role::operator+(Role&& role)
{
    Role other(*this);
    other._BitSet(std::move(role.role_));
    return other;
}
Role Role::operator+(Role::Type&& type)
{
    Role other(*this);
    other._BitSet(std::forward<Role::Type>(type));
    return other;
}
Role& Role::operator+=(Role& role)
{
    _BitSet(std::move(role.role_));
    return *this;
}
Role& Role::operator+=(Role&& role)
{
    _BitSet(std::move(role.role_));
    return *this;
}
Role& Role::operator+=(Role::Type&& type)
{
    _BitSet(std::forward<Role::Type>(type));
    return *this;
}

Role Role::operator-(Role& role)
{
    Role other(*this);
    other._BitUnset(std::move(role.role_));
    return other;
}
Role Role::operator-(Role&& role)
{
    Role other(*this);
    other._BitUnset(std::move(role.role_));
    return other;
}
Role Role::operator-(Role::Type&& type)
{
    Role other(*this);
    other._BitUnset(std::forward<Role::Type>(type));
    return other;
}
Role& Role::operator-=(Role& role)
{
    _BitUnset(std::move(role.role_));
    return *this;
}
Role& Role::operator-=(Role&& role)
{
    _BitUnset(std::move(role.role_));
    return *this;
}
Role& Role::operator-=(Role::Type&& type)
{
    _BitUnset(std::forward<Role::Type>(type));
    return *this;
}

Role& Role::RoleAdd(Role& role)
{
    _BitSet(std::move(role.role_));
    return *this;
}
Role& Role::RoleAdd(Role&& role)
{
    _BitSet(std::move(role.role_));
    return *this;
}
Role& Role::RoleAdd(Role::Type&& type)
{
    _BitSet(std::forward<Role::Type>(type));
    return *this;
}
Role& Role::RoleDelete(Role& role)
{
    _BitUnset(std::move(role.role_));
    return *this;
}
Role& Role::RoleDelete(Role&& role)
{
    _BitUnset(std::move(role.role_));
    return *this;
}
Role& Role::RoleDelete(Role::Type&& type)
{
    _BitUnset(std::forward<Role::Type>(type));
    return *this;
}

bool Role::Has(Role& role)
{
    return Has(std::move(role));
}
bool Role::Has(Role&& role)
{
    return _BitCheck(std::move(role.role_));
}
bool Role::Has(Role::Type&& type)
{
    return _BitCheck(std::move(type));
}
bool Role::Has(std::vector<Role::Type>&& roles)
{
    for (auto type : roles) {
        if (!_BitCheck(std::move(type))) {
            return false;
        }
    }
    return true;
}

void Role::_BitSet(Role::Type&& type)
{

}
void Role::_BitSet(unsigned long long && bitset)
{

}
void Role::_BitUnset(Role::Type&& type)
{

}
void Role::_BitUnset(unsigned long long && bitset)
{

}
bool Role::_BitCheck(Role::Type&& type)
{
    return true;
}
bool Role::_BitCheck(unsigned long long && bitset)
{
    return true;
}

}
