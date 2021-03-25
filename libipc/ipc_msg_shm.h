#ifndef __MSG_SHM_H__
#define __MSG_SHM_H__

#include "shm/shm_posix.h"
#include "sem/sem_posix.h"

#include "msg.h"
#include "ipc_log.h"

namespace infra::ipc::msg {

class MsgShm : public Msg {
public:
    typedef struct _MsgShmHead {
        size_t msg_num;
        size_t free_size;
        size_t data_start;
        size_t data_end;
        size_t msg_head;
        size_t msg_tail;
    } MsgShmHead;
public:
    MsgShm();
    MsgShm(std::string path);
    ~MsgShm();

    IpcRet Create(size_t size, mode_t mode);
    IpcRet Destroy();
    IpcRet Open(IpcMode mode);
    IpcRet Close();

    size_t GetMsgNum();
    size_t GetFreeSize();
    IpcRet GetRet();

    size_t Recv(void* data, size_t data_len, timer::Time* overtime);
    size_t Send(void* data, size_t data_len, timer::Time* overtime);
//private:
public:
    shm::ShmPosix shm_;
    sem::SemPosix sem_;
    IpcRet ret_;

    MsgShmHead* p_shm_head_ = {NULL};

    size_t _align(size_t raw_size, size_t align_size);
};

}

#endif
