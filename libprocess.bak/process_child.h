#ifndef __PROCESS_CHILD_H__
#define __PROCESS_CHILD_H__

#include <string>

#include "timer_time.h"
#include "sock_fd.h"

#include "process_log.h"
#include "process_return.h"
#include "process_id.h"
#include "process_state.h"
#include "process_role.h"

#define ProcessChildSockFDIndex (1)

namespace process {

class ProcessChild {
public:
    ProcessChild();
    ProcessChild(std::string name, ProcessID&& pid);
    ProcessChild(ProcessChild& other);
    ~ProcessChild();

    ProcessID& GetPid();
    std::string GetName();
    ProcessRole& GetRole();
    ProcessState& GetState();
    void (*GetDeadCallback())(int*);
    sock::SockFD& GetFD();

    ProcessChild& SetState(ProcessState state);
    ProcessChild& SetDeadCallback(void (*dead_callback)(int*));
    ProcessChild& SetFD(sock::SockFD& fd);
    ProcessChild& SetFD(sock::SockFD&& fd);

    ProcessRet SetSendBlock(timer::Time* overtime);
    ProcessRet SetRecvBlock(timer::Time* overtime);
    ProcessRet SetNonBlock();

    ProcessRet Send();
    ProcessRet Recv();

private:
    ProcessID       pid_;
    std::string     name_;
    ProcessRole     role_;
    ProcessState    state_;

    bool init_flag_;
    sock::SockFD fd_;
    void (*dead_callback_)(int*);
};

}

#endif
