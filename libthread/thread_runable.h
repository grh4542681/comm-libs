#ifndef __THREAD_RUNABLE_H__
#define __THREAD_RUNABLE_H__

#include "thread_log.h"
#include "thread_return.h"

namespace infra::thread {

class Runable {
public:
    Runable() {
    }

    virtual ~Runable() { }

    virtual ThreadRet Main() {

    }

    ThreadRet Run() {
        main_ = ThreadTemplate<Runable, decltype(&thread::Runable::Main), ThreadRet>(this, &thread::Runable::Main);
        ThreadRet ret = main_.Run();
        if (ret != thread::ThreadRet::SUCCESS) {
            THREAD_ERROR("Start io auto select thread error : %s", ret.Message().c_str());
            return ret;
        }
        main_.Detach(); 
        return ThreadRet::SUCCESS;
    }

    bool IsRunning() {
        return main_.IsRunning();
    }

    ThreadInfo* GetThreadInfo() {
        return main_.GetThreadInfo();
    }
protected:
    ThreadTemplate<Runable, decltype(&thread::Runable::Main), ThreadRet> main_;
};

}

#endif
