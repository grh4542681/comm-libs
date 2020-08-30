#include <stdio.h>
#include "mempool.h"
#include "mempool_center.h"

namespace mempool {

thread_local Mempool* Mempool::pInstance = NULL;

Mempool::Mempool()
{
}

Mempool::~Mempool()
{

}

void Mempool::Report(std::stringstream& ss)
{

}

void Mempool::Report(file::File& fd)
{

}

void Mempool::ReportCenter(file::File& fd)
{

}

void Mempool::ReportThread(file::File& fd)
{
    threadcache_.Report(fd);
}

//private
void* Mempool::_malloc(size_t size)
{
    return threadcache_.Alloc(size);
}

void Mempool::_free(void* ptr)
{
    threadcache_.Free(ptr);
}

Mempool* Mempool::getInstance()
{
    if (!pInstance) {
        pInstance = new Mempool();
    }
    return pInstance;
}

void Mempool::freeInstance()
{
    if (pInstance) {
        delete pInstance;
        pInstance = NULL;
    }
}

}// namespace pub end
