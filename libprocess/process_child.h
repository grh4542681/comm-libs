#ifndef __PROCESS_CHILD_H__
#define __PROCESS_CHILD_H__

#include <string>

#include "timer_time.h"
#include "io_fd.h"

#include "process_log.h"
#include "process_return.h"
#include "process_id.h"
#include "process_state.h"
#include "process_role.h"

#define ChildSockFDIndex (1)

namespace process {

class infra::Child : io::FD {
public:
    typedef std::function<void(int*)> ChildDeadCallback_t;
public:
    Child();
    Child(std::string name, ID&& pid);
    Child(Child& other);
    ~Child();

    ID& GetPid();
    std::string GetName();
    Role& GetRole();
    State& GetState();
    ChildDeadCallback_t GetDeadCallback();

    Child& SetRole(Role&& role);
    Child& SetState(State&& state);
    Child& SetDeadCallback(ChildDeadCallback_t& dead_callback);

private:
    ID          pid_;
    std::string name_;
    Role        role_;
    State       state_;

    ChildDeadCallback_t dead_callback_;
    void (*dead_callback_)(int*);
};

}

#endif
