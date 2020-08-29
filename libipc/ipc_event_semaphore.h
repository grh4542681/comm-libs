#ifndef __SEM_EVENT_SEMAPHORE_H__
#define __SEM_EVENT_SEMAPHORE_H__

#include "sem/sem.h"

#include "event_fd.h"

namespace ipc::event {

class SemEvent : public sem::Sem {
public:
    SemEvent();
    SemEvent(std::string name);
    ~SemEvent();

    EventFD& GetEFD();

    bool IsExist();
    IpcRet Create([[maybe_unused]] size_t semnum, [[maybe_unused]] mode_t mode);
    IpcRet Destroy();
    IpcRet Open(IpcMode mode) {
        return IpcRet::ENONSUPPORT;
    }
    IpcRet Close() {
        return IpcRet::ENONSUPPORT;
    }

protected:
    IpcRet _p(size_t sem_index, timer::Time* overtime);
    IpcRet _v(size_t sem_index);

private:
    EventFD fd_;
};

}


#endif
