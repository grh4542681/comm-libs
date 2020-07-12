#ifndef __SHM_ARRAY_H__
#define __SHM_ARRAY_H__

#include <string>

#include "shm_posix.h"

namespace ipc::shm {

template < typename T >
class ShmArray {
public:
    typedef struct _ShmArrayHead {
        std::string shm_path_;
        std::string shm_name_;

        size_t real_size_;
        size_t head_size_;
        size_t object_size_;

        bool rw_lock_enable_;
        bool mutex_lock_enable_;

        Time create_time_;
        Time last_access_time_;
        pid_t last_access_proc_;
        pid_t last_access_thread_;
    } ShmArrayHead;
public:
    ShmArray();
    ~ShmArray();

    IpcRet Create(size_t obj_size, mode_t mode);
    IpcRet Destroy();
    IpcRet Open(IpcMode mode);
    IpcRet Close();

private:
    ShmArrayHead p_shm_head_;
    ShmPosix shm_;
};

}

#endif
