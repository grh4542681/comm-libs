#ifndef __PROCESS_CHILD_H__
#define __PROCESS_CHILD_H__

#include <string>
#include <functional>

#include "object.h"
#include "timer_time.h"
#include "io_fd.h"

#include "process_log.h"
#include "process_return.h"
#include "process_id.h"
#include "process_state.h"
#include "process_role.h"
#include "process_relation.h"

#define ChildSockFDIndex (1)

namespace infra::process {

class Child : virtual public base::Object, public io::FD {
public:
    typedef std::function<void(int*)> ChildDeadCallback_t;
public:
    Child();
    Child(std::string name, ID&& pid);
    Child(Child& other);
    ~Child();

    FD* Clone();
    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

    ID& GetPid();
    std::string GetName();
    Role& GetRole();
    State& GetState();
    Relation& GetRelation();
    ChildDeadCallback_t GetDeadCallback();

    Child& SetState(State&& state);
    Child& SetDeadCallback(ChildDeadCallback_t& dead_callback);

    Child& AddRole(Role&& role);
    Child& DelRole(Role&& role);

    Child& AddRelation(Relation&& relation);
    Child& DelRelation(Relation&& relation);

private:
    ID           pid_;
    std::string  name_;
    Role         role_;
    State        state_;
    Relation     relation_;

    ChildDeadCallback_t dead_callback_;
};

}

#endif
