#ifndef __MEMPOOL_THREAD_CACHE_H__
#define __MEMPOOL_THREAD_CACHE_H__

#include <sys/types.h>

#include "mempool_center.h"
#include "mempool_freelist.h"
#include "mempool_busylist.h"

namespace file {
class File;
}

namespace mempool {

class MempoolThreadCache {
public:
    MempoolThreadCache();
    ~MempoolThreadCache();

    MempoolRet Init();

    void* Alloc(size_t size);
    void Free(void* ptr);

    void Report(int fd);
    void Report(file::File& fd);
private:
    bool init_flag_;
    pid_t tid_;
    MempoolCenter* center_;
    MempoolFreeList free_list_;
    MempoolBusyList busy_list_;
};

} //namespace ned

#endif
