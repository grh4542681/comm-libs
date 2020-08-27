#ifndef __WEAK_KEY_H__
#define __WEAK_KEY_H__

#include <tuple>
#include <utility>

namespace infra::container {

template < typename ... Args > class WeakKey {
public:
    WeakKey(Args&& ... args) {
        tuple_key_ = std::make_tuple(std::forward<Args>(args)...);
    }
    WeakKey(const WeakKey& other) {
        tuple_key_ = other.tuple_key_;
    }
    ~WeakKey() { }

    const WeakKey& operator=(const WeakKey& other) {
        tuple_key_ = other.tuple_key_;
    }

    bool operator<(const WeakKey& other) {
        for (int i = 0; i < std::tuple_size<std::tuple<Args...>>::value; i++) {
            if (std::get<i>(tuple_key_) == std::get<i>(other.tuple_key_)) {
                return false;
            }
        }
        return (tuple_key_ < other.tuple_key_);
    }
protected:
    std::tuple<Args...> tuple_key_;
};

}

#endif
