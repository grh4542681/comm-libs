#include "thread_role.h"

namespace infra::thread {

std::vector<std::tuple<Role::Type, std::string, std::string>> Role::TypeDescription = {
    //   type        string          describe
    { Type::PoolWorker, "PoolWorker", "Process pool worker" },
    { Type::PoolKeeper, "PoolKeeper", "Process pool manager" },
};

Role::Role() : _bitmap(static_cast<std::underlying_type<Role::Type>::type>(Role::Type::EnumEnd)) { }
Role::Role(Role::Type&& type) : _bitmap(static_cast<std::underlying_type<Role::Type>::type>(Role::Type::EnumEnd))
{
    RoleAdd(std::forward<Role::Type>(type));
}
Role::Role(std::vector<Role::Type>&& roles) : _bitmap(static_cast<std::underlying_type<Role::Type>::type>(Role::Type::EnumEnd))
{
    for(auto type : roles) {
        RoleAdd(std::move(type));
    }
}
Role::Role(const Role& other)
{
    _bitmap = other._bitmap;
}
Role::Role(const Role&& other)
{
    _bitmap = other._bitmap;
}
Role::~Role() { }

Role& Role::operator=(const Role& other)
{
    _bitmap = other._bitmap;
    return *this;
}

Role Role::operator+(Role& role)
{
    Role other(*this);
    other += role;
    return other;
}
Role Role::operator+(Role&& role)
{
    Role other(*this);
    other += role;
    return other;
}
Role Role::operator+(Role::Type&& type)
{
    Role other(*this);
    other += std::forward<Role::Type>(type);
    return other;
}
Role& Role::operator+=(Role& role)
{
    _bitmap.Set(std::move(role._bitmap));
    return *this;
}
Role& Role::operator+=(Role&& role)
{
    _bitmap.Set(std::move(role._bitmap));
    return *this;
}
Role& Role::operator+=(Role::Type&& type)
{
    _bitmap.Set(static_cast<std::underlying_type<Role::Type>::type>(type));
    return *this;
}

Role Role::operator-(Role& role)
{
    Role other(*this);
    other -= role;
    return other;
}
Role Role::operator-(Role&& role)
{
    Role other(*this);
    other -= role;
    return other;
}
Role Role::operator-(Role::Type&& type)
{
    Role other(*this);
    other -= std::forward<Role::Type>(type);
    return other;
}
Role& Role::operator-=(Role& role)
{
    _bitmap.Unset(std::move(role._bitmap));
    return *this;
}
Role& Role::operator-=(Role&& role)
{
    _bitmap.Unset(std::move(role._bitmap));
    return *this;
}
Role& Role::operator-=(Role::Type&& type)
{
    _bitmap.Unset(static_cast<std::underlying_type<Role::Type>::type>(type));
    return *this;
}

Role& Role::RoleAdd(Role& role)
{
    return RoleAdd(std::move(role));
}
Role& Role::RoleAdd(Role&& role)
{
    _bitmap.Set(std::move(role._bitmap));
    return *this;
}
Role& Role::RoleAdd(Role::Type&& type)
{
    _bitmap.Set(static_cast<std::underlying_type<Role::Type>::type>(type));
    return *this;
}
Role& Role::RoleDelete(Role& role)
{
    return RoleDelete(std::move(role));
}
Role& Role::RoleDelete(Role&& role)
{
    _bitmap.Unset(std::move(role._bitmap));
    return *this;
}
Role& Role::RoleDelete(Role::Type&& type)
{
    _bitmap.Unset(static_cast<std::underlying_type<Role::Type>::type>(type));
    return *this;
}

bool Role::Has(Role& role)
{
    return Has(std::move(role));
}
bool Role::Has(Role&& role)
{
    return _bitmap.Isset(std::move(role._bitmap));
}
bool Role::Has(Role::Type&& type)
{
    return _bitmap.Isset(static_cast<std::underlying_type<Role::Type>::type>(type));
}
bool Role::Has(std::vector<Role::Type>&& roles)
{
    for (auto type : roles) {
        if (!Has(std::move(type))) {
            return false;
        }
    }
    return true;
}

}
