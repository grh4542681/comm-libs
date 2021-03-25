#ifndef __MEMPOOL_OSPROXY_H__
#define __MEMPOOL_OSPROXY_H__

#include <stdlib.h>

namespace infra::mempool {

class MempoolOsProxy {
public:
    MempoolOsProxy();
    ~MempoolOsProxy();

    static void* Alloc(size_t size);
    static void Free(void* ptr);
};

};

#endif
