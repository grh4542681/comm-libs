#include <string.h>

#include "mempool_busylist.h"
#include "file.h"

namespace infra::mempool {

MempoolBusyList::MempoolBusyList()
{
    busy_map_.clear();
}

MempoolBusyList::~MempoolBusyList()
{
    busy_map_.clear();
}

MempoolRet MempoolBusyList::Insert(void* ptr, MempoolItemOri ori)
{
    struct MempoolBusyItem item;
    item.ptr_ = ptr;
    item.alloc_time_ = time(NULL);
    item.ori_ = ori;

    auto it = busy_map_.find(ptr);
    if (it != busy_map_.end()) {
        return MempoolRet::EBUSYLISTDUPADDRESS;
    }

    std::pair<std::map<void*, MempoolBusyItem>::iterator, bool> ret;
    ret = busy_map_.insert(std::pair<void*, MempoolBusyItem>(ptr, item));
    if (ret.second==false) {
        return MempoolRet::EBUSYLISTINSERT;
    }

    return MempoolRet::SUCCESS;
}

MempoolRet MempoolBusyList::Remove(void* ptr)
{
    auto it = busy_map_.find(ptr);
    if (it == busy_map_.end()) {
        return MempoolRet::EBUSYLISTNOTFOUND;
    }

    busy_map_.erase(it);
    return MempoolRet::SUCCESS;
}

MempoolItemOri MempoolBusyList::Origin(void* ptr)
{
    auto it = busy_map_.find(ptr);
    if (it == busy_map_.end()) {
        return MempoolItemOri::NONE;
    }
    return it->second.ori_;
}

unsigned int MempoolBusyList::Size()
{
    return busy_map_.size();
}

MempoolRet MempoolBusyList::Clear()
{
    return MempoolRet::SUCCESS;
}

void MempoolBusyList::Report(file::File& fd)
{
    char line[1024];
    for (auto it : busy_map_) {
        memset(line, 0x00, sizeof(line));
        sprintf(line, "Address: %p\t alloctime: %lu\t", it.second.ptr_, it.second.alloc_time_);
        switch (it.second.ori_) {
            case MempoolItemOri::OS:
                strcat(line, " ORI: OS");
                break;
            case MempoolItemOri::POOL:
                strcat(line, " ORI: POOL");
                break;
            default:
                strcat(line, " ORI: Unknow");
                break;
        }
        strcat(line, "\n");
        fd.GetFileFD().Write(line, sizeof(line));
    }
}

};
