#ifndef __CONTAINER_MULTIKEY_VECTOR_H__
#define __CONTAINER_MULTIKEY_VECTOR_H__

#include <vector>

#include "object"

namespace infra::container {

class VIndex {
public:
    VIndex() { }
    virtual ~Vindex() { }
}

template < typename E >
class IndexVector : virtual public ::infra::base::object {
public:
    IndexVector() { }
    ~IndexVector() { }

    template < typename K > Return CreateIndex(std::string name) {

    }
    template < typename K > Return RemoveIndex(std::string name) {

    }
private:
    std::map<std::string, Index>
    std::vector<E> date_;
};

}

#endif
