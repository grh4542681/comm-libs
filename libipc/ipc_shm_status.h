#ifndef __SHM_STATUS_H__
#define __SHM_STATUS_H__

namespace ipc::shm {

enum class ShmStatus {
    UNKNOW = 0x00,
    OPEN,
    CLOSE,
    CREATE,
    DESTROY,
};

}

#endif