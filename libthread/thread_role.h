#ifndef __THREAD_ROLE_H__
#define __THREAD_ROLE_H__

#include <string>
#include <tuple>
#include <vector>

#include "object.h"
#include "container_bitmap.h"

namespace infra::thread {

class Role : virtual public base::Object {
public:
    enum class Type : size_t {
        EnumBegin = 0,
        Normal,
        Parent,
        Child,
        PoolWorker,
        PoolKeeper,
        EnumEnd,
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
    ::infra::container::Bitmap _bitmap;
};

}

#endif
