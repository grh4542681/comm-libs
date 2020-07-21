#ifndef __PROCESS_GROUP_WORKER_H__
#define __PROCESS_GROUP_WORKER_H__

#include "sock_fd.h"
#include "process_id.h"

namespace process::group {

class ProcessGroupWorker {
public:
    ProcessGroupWorker();
    ProcessGroupWorker(ProcessID& id, sock::SockFD& fd);
    ProcessGroupWorker(const ProcessGroupWorker& other);
    ~ProcessGroupWorker();

private:
    ProcessID pid_;
    sock::SockFD fd_;
};

}

#endif
