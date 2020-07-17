#ifndef __ALLOCTOR_H__
#define __ALLOCTOR_H__

#include <string.h>
#include <memory>

namespace infra {

class Allocator {
public:
    Allocator() {
    }
    virtual ~Allocator() { }

    void* Malloc(size_t size) {
        return _malloc(size);
    }
    void Free(void* data) {
        return _free(data);
    }

    template < typename T >
    std::allocator<T> GetAllocator() {
        return std::allocator<T>();
    }

    template < typename T, typename ... Args>
    T* Allocate(Args&& ... args) {
        T* ptr = (T*)_malloc(sizeof(T));
        if (!ptr) {
            return NULL;
        } else {
            return (new(ptr) T(std::forward<Args>(args)...));
        }
    }
    template< typename T >
    void Deallocate(T* data) {
        data->~T();
        _free(data);
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

    virtual void* _malloc(size_t size) {
        return malloc(size);
    }
    virtual void _free(void* data) {
        return free(data);
    }
};

}

#endif
