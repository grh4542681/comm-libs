#ifndef __PROCESS_PARENT_H__
#define __PROCESS_PARENT_H__

#include <string>

#include "timer_time.h"
#include "sock_fd.h"

#include "process_log.h"
#include "process_return.h"
#include "process_id.h"
#include "process_state.h"
#include "process_role.h"

#define ProcessParentSockFDIndex (0)

namespace infra::process {

class ProcessParent {
public:
    ProcessParent();
    ProcessParent(std::string name, ProcessID& pid);
    ProcessParent(std::string name, ProcessID&& pid);
    ProcessParent(ProcessParent& other);
    ~ProcessParent();

    ProcessID& GetPid();
    std::string GetName();
    sock::SockFD& GetFD();

    ProcessParent& SetFD(sock::SockFD& fd);
    ProcessParent& SetFD(sock::SockFD&& fd);

    ProcessRet SetSendBlock(timer::Time* overtime);
    ProcessRet SetRecvBlock(timer::Time* overtime);
    ProcessRet SetNonBlock();

    ProcessRet Send();
    ProcessRet Recv();

private:
    ProcessID       pid_;
    std::string     name_;

    bool init_flag_;
    sock::SockFD fd_;
};

}

#endif
