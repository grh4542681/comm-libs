#include "mempool_freelist.h"

namespace infra::mempool {

MempoolFreeList::MempoolFreeList()
{
}

MempoolFreeList::~MempoolFreeList()
{

}

void* MempoolFreeList::Alloc(size_t size)
{

}

void MempoolFreeList::Free(void* ptr)
{

}

MempoolRet MempoolFreeList::Clear()
{
    return MempoolRet::SUCCESS;
}

} //namespace infra::end
