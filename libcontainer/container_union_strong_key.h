#ifndef __UNION_STRONG_KEY_H__
#define __UNION_STRONG_KEY_H__

#include <tuple>
#include <utility>

#include "object.h"

namespace infra::container {

template < typename ... Args >
class UnionStrongKey : virtual public infra::base::Object {
public:
    UnionStrongKey(Args&& ... args) {
        tuple_key_ = std::make_tuple(std::forward<Args>(args)...);
    }
    UnionStrongKey(const UnionStrongKey& other) {
        tuple_key_ = other.tuple_key_;
    }
    ~UnionStrongKey() { }

    const UnionStrongKey& operator=(const UnionStrongKey& other) {
        tuple_key_ = other.tuple_key_;
    }

    bool operator<(const UnionStrongKey& other) {
        return (tuple_key_ < other.tuple_key_);
    }
protected:
    std::tuple<Args...> tuple_key_;
};

}

#endif
