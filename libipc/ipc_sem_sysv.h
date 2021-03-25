#ifndef __SEM_SYSV_H___
#define __SEM_SYSV_H___

#include <sys/types.h>
#include <errno.h>

#include "ipc_return.h"
#include "sem.h"

namespace infra::infra::ipc::sem {

/**
* @brief - System V semphore set.
*/
class SemSysV : public Sem {
public:
    SemSysV();
    SemSysV(std::string path);
    ~SemSysV();

    bool IsExist();
    IpcRet Create(size_t semnum, mode_t mode);
    IpcRet Destroy();
    IpcRet Open(IpcMode mode);
    IpcRet Close();

protected:
    IpcRet _p(size_t sem_index, timer::Time* overtime);
    IpcRet _v(size_t sem_index);

private:
    union semun {
        int              val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                       (Linux-specific) */
    };

    int semid_; ///< Semaphore set id.
    key_t key_; ///< Semaphore set key.
};

} // namespace infra::infra::ipc

#endif
