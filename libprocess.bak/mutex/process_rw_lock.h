#ifndef __PROCESS_RW_LOCK_H__
#define __PROCESS_RW_LOCK_H__

#include <pthread.h>

#include "timer_time.h"
#include "process_return.h"

namespace infra::process::mutex {

class ProcessRWLock{
public:
    ProcessRWLock(){}
    ~ProcessRWLock();

    void setNonBlock(bool);
    bool getNonBlock();

    ProcessRet RLock(timer::Time* overtime);
    ProcessRet WLock(timer::Time* overtime);
    ProcessRet UnLock();
private:
    bool init_flag_;
    bool nonblock_flag_;
    pthread_rwlock_t rwlock_;

    static ProcessRet _errno2ret(int ierrno) {
        switch (ierrno) {
            case 0:
                return ThreadRet::SUCCESS;
            case EBUSY:
            case EINVAL:
            case EAGAIN:
            case EDEADLK:
            default:
                return ThreadRet::ERROR;
        }
    }

};

}

#endif
