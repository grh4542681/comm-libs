#ifndef _THREAD_INFO_H__
#define _THREAD_INFO_H__

#include <sys/types.h>
#include <string>
#include <thread>
#include <memory>

#include "mempool.h"

#include "thread_return.h"
#include "thread_id.h"

namespace thread {
template <typename H, typename F, typename R> class ThreadTemplate;
class ThreadInfo {
public:
    friend class mempool::Mempool;
    template <typename H, typename F, typename R> friend class ThreadTemplate;
public:
    static ThreadInfo* getInstance();
    static void freeInstance();

    ThreadID& GetTid();
    std::string& GetThreadName();
    void SetThreadName(const char* name);

    ThreadRet Register2Process();
    ThreadRet Unregister2Process();

private:
    ThreadInfo();
    ~ThreadInfo();

    ThreadID tid_;
    std::shared_ptr<std::thread> thread_;
    bool exit_;
    std::string thread_name_;

    thread_local static ThreadInfo* pInstance;
};


};

#endif
