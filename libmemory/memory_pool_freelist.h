#ifndef __MEMPOOL_FREELIST_H__
#define __MEMPOOL_FREELIST_H__

#include "mempool_log.h"
#include "mempool_return.h"
#include "mempool_sizemap.h"
#include "mempool_memorylist.h"

namespace mempool {

class MempoolFreeList {
public:
    MempoolFreeList();
    ~MempoolFreeList();

    void* Alloc(size_t size);
    void Free(void* ptr);
    
    MempoolRet Clear();
private:
//    MempoolMemoryList list_array_[MempoolSizeMap::g_sobj_array_size];

};

} //namespace end

#endif
