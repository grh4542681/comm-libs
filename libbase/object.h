#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <new>
#include "allocator.h"

namespace infra::base {

class Object {
public:
    Object() { }
    virtual ~Object() { }


    static void* operator new(size_t sz) {
        return ::infra::base::Allocator::Instance().Malloc(sz);
    }
    static void operator delete(void* p) {
        ::infra::base::Allocator::Instance().Free(p);
    }
    static void *operator new[](size_t sz) {
        return ::infra::base::Allocator::Instance().Malloc(sz);
    }
    static void operator delete[](void *p) {
        ::infra::base::Allocator::Instance().Free(p);
    }
};

}

#endif
