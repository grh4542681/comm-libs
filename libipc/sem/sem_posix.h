#ifndef __SEM_POSIX_H__
#define __SEM_POSIX_H__

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <map>
#include <string>

#include "sem.h"

namespace ipc::sem {

class SemPosix : public Sem {
public:
    SemPosix();
    SemPosix(std::string path);
    ~SemPosix();

    bool IsExist();
    IpcRet Create(size_t semnum, mode_t mode);
    IpcRet Destroy();
    IpcRet Open(IpcMode mode);
    IpcRet Close();

protected:
    IpcRet _p(size_t sem_index, timer::Time* overtime);
    IpcRet _v(size_t sem_index);

private:
    std::map<std::string, sem_t*> semset_;
};

}

#endif
