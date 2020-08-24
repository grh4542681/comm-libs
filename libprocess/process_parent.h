#ifndef __PROCESS_PARENT_H__
#define __PROCESS_PARENT_H__

#include <string>

#include "object.h"
#include "io_fd.h"
#include "timer_time.h"

#include "process_log.h"
#include "process_return.h"
#include "process_id.h"
#include "process_state.h"
#include "process_role.h"
#include "process_relation.h"

#define ParentSockFDIndex (0)

namespace infra::process {

class Parent : virtual public base::Object, public io::FD {
public:
    Parent();
    Parent(std::string name, ID& pid);
    Parent(std::string name, ID&& pid);
    Parent(const Parent& other);
    ~Parent();

    FD* Clone();
    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

    ID& GetPid();
    std::string GetName();
    Role GetRole();
    State GetState();

    Parent& SetState(State&& state);

    Parent& AddRole(Role&& role);
    Parent& DelRole(Role&& role);

    Parent& AddRelation(Relation&& relation);
    Parent& DelRelation(Relation&& relation);

private:
    ID              pid_;
    std::string     name_;
    Role            role_;
    State           state_;
    Relation        relation_;
};

}

#endif
