#ifndef _THREAD_INFO_H__
#define _THREAD_INFO_H__

#include <sys/types.h>
#include <string>
#include <thread>
#include <memory>

#include "thread_return.h"
#include "thread_id.h"
#include "thread_state.h"
#include "thread_role.h"
#include "thread_scopedlock.h"

namespace infra::thread {
template <typename H, typename F> class Template;
class Handler : virtual public ::infra::base::Object {
public:
    template <typename H, typename F> friend class Template;
public:
    Handler& SetThreadName(const char* name);
    Handler& SetState(State&& state);
    Handler& SetRole(Role&& role);

    std::string GetThreadName();
    ID GetTid();
    State GetState();
    Role GetRole();

    Handler& AddRole(Role&& role);
    Handler& DelRole(Role&& role);

    Return Register();
    Return Unregister();
    Return Join();
    Return Detach();

private:
    Handler();
    ~Handler();

    Handler& SetTid(ID&& tid);

    std::string name_;
    ID tid_;
    Role role_;
    State state_;
    Mutex mutex_;
    std::thread thread_;
};


};

#endif
