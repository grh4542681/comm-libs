#ifndef __SHM_ALLOCTOR_H__
#define __SHM_ALLOCTOR_H__

#include "alloctor.h"

namespace ipc::shm {

class ShmAlloctor : public base::Alloctor {
public:
    ShmAlloctor() : Alloctor() { }
    ~ShmAlloctor() { }

    template < typename T, typename ... Args>
    T* Allocate(Args&& ... args) {
        printf("aaaaaaa\n");
        return new(std::nothrow) T(std::forward<Args>(args)...);
    }

};

}

#endif
