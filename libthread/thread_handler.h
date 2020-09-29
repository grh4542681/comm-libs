#ifndef _THREAD_INFO_H__
#define _THREAD_INFO_H__

#include <sys/types.h>
#include <string>
#include <thread>
#include <memory>

#include "thread_return.h"
#include "thread_id.h"

namespace infra::thread {
class Handler {
public:
    ID& GetTid();
    std::string& GetThreadName();
    void SetThreadName(const char* name);

    Return Register();
    Return Unregister();
    Return Join();
    Return Detach();

private:
    Handler();
    ~Handler();

    ID tid_;
    std::shared_ptr<std::jthread> thread_;
    std::string thread_name_;
};


};

#endif
