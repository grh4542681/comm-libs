#ifndef __MEMPOOL_BUSYLIST_H__
#define __MEMPOOL_BUSYLIST_H__

#include <pthread.h>
#include <map>
#include <time.h>

#include "mempool_log.h"
#include "mempool_return.h"
#include "mempool_itemori.h"

namespace infra::infra::file {
class File;
}

namespace infra::infra::mempool {

class MempoolBusyList {
public:
    struct MempoolBusyItem {
        void* ptr_;
        time_t alloc_time_;
        MempoolItemOri ori_;
    };
public:
    MempoolBusyList();
    ~MempoolBusyList();

    MempoolRet Insert(void* ptr, MempoolItemOri ori);
    MempoolRet Remove(void* ptr);
    MempoolItemOri Origin(void* ptr);
    
    unsigned int Size();
    MempoolRet Clear();

    void Report(file::File& fd);

private:
    std::map<void*, MempoolBusyItem> busy_map_;

};

};

#endif
