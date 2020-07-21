#ifndef __PROCESS_PARENT_H__
#define __PROCESS_PARENT_H__

#include <string>

#include "io_fd.h"
#include "timer_time.h"

#include "process_log.h"
#include "process_return.h"
#include "process_id.h"
#include "process_state.h"
#include "process_role.h"

#define ParentSockFDIndex (0)

namespace infra::process {

class Parent : io::FD {
public:
    Parent();
    Parent(std::string name, ID& pid);
    Parent(std::string name, ID&& pid);
    Parent(Parent& other);
    ~Parent();

    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

    ID& GetPid();
    std::string GetName();
    Role GetRole();
    State GetState();

    Parent& SetRole(Role&& role);
    Parent& SetState(State&& state);

private:
    ID              pid_;
    std::string     name_;
    Role            role_;
    State           state_;
};

}

#endif
