#ifndef __UNION_WEAK_KEY_H__
#define __UNION_WEAK_KEY_H__

#include <tuple>
#include <utility>

#include "object.h"

namespace infra::container {

template < typename ... Args >
class UnionWeakKey : virtual public infra::base::Object {
public:
    UnionWeakKey(Args&& ... args) {
        tuple_key_ = std::make_tuple(std::forward<Args>(args)...);
    }
    UnionWeakKey(const UnionWeakKey& other) {
        tuple_key_ = other.tuple_key_;
    }
    ~UnionWeakKey() { }

    const UnionWeakKey& operator=(const UnionWeakKey& other) {
        tuple_key_ = other.tuple_key_;
    }

    bool operator<(const UnionWeakKey<Args...>& other) const {
//        for (long unsigned i = 0; i < std::tuple_size<std::tuple<Args...>>::value; i++) {
//            if (std::get<(const long unsigned)(i)>(tuple_key_) ==
//                std::get<(const long unsigned)(i)>(other.tuple_key_)) {
//                return false;
//            }
//        }
        return (tuple_key_ < other.tuple_key_);
    }
protected:
    std::tuple<Args...> tuple_key_;
};

}

#endif
