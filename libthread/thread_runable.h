#ifndef __THREAD_RUNABLE_H__
#define __THREAD_RUNABLE_H__

#include "thread_log.h"
#include "thread_return.h"
#include "thread_template.h"

namespace thread {

class ThreadRunable {
public:
    ThreadRunable() {
    }

    virtual ~ThreadRunable() { }

    virtual ThreadRet Main() {

    }

    ThreadRet Run() {
        main_ = ThreadTemplate<ThreadRunable, decltype(&thread::ThreadRunable::Main), ThreadRet>(this, &thread::ThreadRunable::Main);
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
    ThreadTemplate<ThreadRunable, decltype(&thread::ThreadRunable::Main), ThreadRet> main_;
};

}

#endif
