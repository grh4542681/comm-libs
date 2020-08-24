#ifndef __PROCESS_ROLE_H__
#define __PROCESS_ROLE_H__

#include <string>
#include <tuple>
#include <vector>

#include "object.h"

namespace infra::process {

class Role : virtual public base::Object {
public:
    enum class Type : unsigned int {
        Normal = 0,
        Parent,
        Child,
        PoolWorker,
        PoolKeeper,
    };
private:
    static std::vector<std::tuple<Role::Type, std::string, std::string>> TypeDescription;
public:
    Role();
    Role(Type&& type);
    Role(std::vector<Type>&& roles);
    Role(const Role&);
    Role(const Role&&);
    ~Role();

    Role& operator=(const Role& other);

    Role operator+(Role& role);
    Role operator+(Role&& role);
    Role operator+(Type&& type);
    Role& operator+=(Role& role);
    Role& operator+=(Role&& role);
    Role& operator+=(Type&& type);

    Role operator-(Role& role);
    Role operator-(Role&& role);
    Role operator-(Type&& type);
    Role& operator-=(Role& role);
    Role& operator-=(Role&& role);
    Role& operator-=(Type&& type);

    Role& RoleAdd(Role& role);
    Role& RoleAdd(Role&& role);
    Role& RoleAdd(Type&& type);
    Role& RoleDelete(Role& role);
    Role& RoleDelete(Role&& role);
    Role& RoleDelete(Type&& type);

    bool Has(Role& role);
    bool Has(Role&& role);
    bool Has(Type&& type);
    bool Has(std::vector<Type>&& roles);

private:
    void _BitSet(Type&& type);
    void _BitSet(unsigned long long && bitset);
    void _BitUnset(Type&& type);
    void _BitUnset(unsigned long long && bitset);
    bool _BitCheck(Type&& type);
    bool _BitCheck(unsigned long long && bitset);

    unsigned long long role_ = 0;
};

}

#endif
