#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "file.h"

#include "mempool_threadcache.h"
#include "mempool_osproxy.h"

namespace mempool {

MempoolThreadCache::MempoolThreadCache()
{
    init_flag_ = false;
    tid_ = (pid_t)syscall(__NR_gettid);
    center_ = NULL;
    if (busy_list_.Clear() != MempoolRet::SUCCESS) {
        MEMPOOL_ERROR("busy list clear error");
    }   
    if (free_list_.Clear() != MempoolRet::SUCCESS) {
        MEMPOOL_ERROR("free list clear error");
    }
    if (Init() == MempoolRet::SUCCESS) {
        init_flag_ = true;
    }
}

MempoolThreadCache::~MempoolThreadCache()
{

}

MempoolRet MempoolThreadCache::Init()
{
    return MempoolRet::ERROR;
}

void* MempoolThreadCache::Alloc(size_t size)
{
    MempoolRet ret = MempoolRet::SUCCESS;
    void* ptr = NULL;

    if (init_flag_) {
        ptr = free_list_.Alloc(size);
        if (!ptr) {
            MEMPOOL_ERROR("Alloc from free list error");
            return NULL;
        }
        if ((ret = busy_list_.Insert(ptr, MempoolItemOri::POOL)) != MempoolRet::SUCCESS) {
            MEMPOOL_ERROR("Insert into bust list error");
            free_list_.Free(ptr);
            ptr = NULL;
            return NULL;
        }
    } else {
        ptr = MempoolOsProxy::Alloc(size);
        if (!ptr) {
            MEMPOOL_ERROR("Alloc from os error");
            return NULL;
        }
        if ((ret = busy_list_.Insert(ptr, MempoolItemOri::OS)) != MempoolRet::SUCCESS) {
            MEMPOOL_ERROR("Insert into bust list error");
            MempoolOsProxy::Free(ptr);
            ptr = NULL;
            return NULL;
        }
    }
    return ptr;
}

void MempoolThreadCache::Free(void* ptr)
{
    if (!ptr) {
        return;
    }
    MempoolItemOri origin = busy_list_.Origin(ptr);
    switch (origin) {
        case MempoolItemOri::OS:
            MempoolOsProxy::Free(ptr);
            busy_list_.Remove(ptr);
            return;
        case MempoolItemOri::POOL:
            free_list_.Free(ptr);
            busy_list_.Remove(ptr);
            return;
        case MempoolItemOri::NONE:
            MEMPOOL_ERROR("Free address %p is untrack address.", ptr);
            return;
        default:
            MEMPOOL_ERROR("Unknow memory origon");
            return;
    }
}

void MempoolThreadCache::Report(int fd)
{
    //Report(file::File(fd));
}

void MempoolThreadCache::Report(file::File& fd)
{
    char line[1024];
    memset(line, 0x00, sizeof(line));
    sprintf(line, "Thread: %d\n", tid_);
    fd.GetFileFD().Write(line, sizeof(line));
    busy_list_.Report(fd);
}

} //namespace end
