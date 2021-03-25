#ifndef __SEM_EVENTFD_H__
#define __SEM_EVENTFD_H__

#include <sys/eventfd.h>

#include <string>

#include "ipc_return.h"
#include "sem/sem.h"

namespace infra::ipc::sem {

class SemEventfd : public Sem {
public:
    SemEventfd();
    SemEventfd(std::string name);
    SemEventfd(std::string name, std::string handle);
    ~SemEvebtfd();

    bool IsExist();
    IpcRet Create(size_t semnum, mode_t mode);
    IpcRet Destroy();
    IpcRet Open(IpcMode mode);
    IpcRet Close();
private:
    IpcRet _p(size_t sem_index, timer::Time* overtime);
    IpcRet _v(size_t sem_index);
};

}

#endif
