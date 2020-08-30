#ifndef __MEMPOOL_ALLOCTOR_H__
#define __MEMPOOL_ALLOCTOR_H__

#include "alloctor.h"
#include "mempool.h"

namespace mempool {

class MempoolAlloctor : public base::Alloctor {
public:
    MempoolAlloctor();
    ~MempoolAlloctor();
    
    //Inherited from Alloctor
    void* _malloc(size_t size);
    void _free(void* ptr);

private:
    Mempool* mp_ = nullptr;
};

}

#endif
