#include "process_group_worker.h"

namespace process::group {

ProcessGroupWorker::ProcessGroupWorker() { }
ProcessGroupWorker::ProcessGroupWorker(ProcessID& id, sock::SockFD& fd) : pid_(id), fd_(fd) { }
ProcessGroupWorker::ProcessGroupWorker(const ProcessGroupWorker& other)
{
    pid_ = other.pid_;
    fd_ = other.fd_;
}

ProcessGroupWorker::~ProcessGroupWorker() { }

}
