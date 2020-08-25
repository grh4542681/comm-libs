#ifndef __ALLOCTOR_H__
#define __ALLOCTOR_H__

#include <string.h>
#include <stdlib.h>
#include <memory>
#include <cassert>
#include <iostream>
#include <unistd.h>

namespace infra::base {

class AllocatorInterface {
public:
    AllocatorInterface() { }
    virtual ~AllocatorInterface() { }

    virtual void* Malloc(size_t size) {
        return malloc(size);
    }
    virtual void Free(void* data) {
        return free(data);
    }
};

class Allocator {
public:
    Allocator() {
        interface_ = new AllocatorInterface();
    }
    virtual ~Allocator() {
        if (interface_) {
            delete interface_;
        }
    }

    static Allocator& Instance() {
        static Allocator instance;
        return instance;
    }

    template < typename I, typename ... Args > Allocator& SetInterface(Args ... args) {
        if (interface_) {
            delete interface_;
        }
        interface_ = new I(std::forward<Args>(args)...);
        return *this;
    }

    void* Malloc(size_t size) {
        assert(interface_);
        return interface_->Malloc(size);
    }
    void Free(void* data) {
        assert(interface_);
        interface_->Free(data);
    }

    template < typename T, typename ... Args>
    T* Allocate(Args&& ... args) {
        T* ptr = (T*)Malloc(sizeof(T));
        if (!ptr) {
            return NULL;
        } else {
            return (new(ptr) T(std::forward<Args>(args)...));
        }
    }
    template< typename T >
    void Deallocate(T* data) {
        data->~T();
        Free(data);
    }

    template < typename T, typename ... Args> static T* Construct(void* ptr, Args&& ... args){
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<Args>(args)...));
    }
    template < typename T, typename ... Args> static T* Reconstruct(T* ptr, Args&& ... args){
        ptr->~T();
        return (new(ptr) T(std::forward<Args>(args)...));
    }
    template < typename T > static void* Destruct(T* ptr) {
        ptr->~T();
        return ptr;
    }

private:
    AllocatorInterface* interface_;
};

}

#endif
