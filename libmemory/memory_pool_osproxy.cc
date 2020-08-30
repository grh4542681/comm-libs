#include <stdlib.h>

#include "mempool_osproxy.h"

namespace mempool {

MempoolOsProxy::MempoolOsProxy()
{

}

MempoolOsProxy::~MempoolOsProxy()
{

}

void* MempoolOsProxy::Alloc(size_t size)
{
    return malloc(size);
}

void MempoolOsProxy::Free(void* ptr)
{
    return free(ptr);
}

}
