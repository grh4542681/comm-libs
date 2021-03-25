#include "process_pool_worker.h"

namespace infra::process::pool {

ProcessPoolWorker::ProcessPoolWorker(sock::SockAddress& worker_address)
{
    sock::SockClient client(&worker_address);
    if (client.Connect() == sock::SockRet::SUCCESS) {
        worker_fd_ = client.GetSockFD();
        select_item_ = SelectItem(this);
        state_ = State::Ready;
    } else {
        state_ = State::Error;
    }
}

ProcessPoolWorker::ProcessPoolWorker(ProcessID& pid, sock::SockFD& worker_fd)
{
    info_.pid_ = pid;
    worker_fd_ = worker_fd;
    state_ = State::Ready;
}

ProcessPoolWorker::~ProcessPoolWorker()
{

}

sock::SockFD& ProcessPoolWorker::GetWorkerFD()
{
    return worker_fd_;
}

ProcessPoolWorker::SelectItem& ProcessPoolWorker::GetSelectItem()
{
    return select_item_;
}

ProcessPoolWorker::State& ProcessPoolWorker::GetState()
{
    return state_;
}

bool ProcessPoolWorker::IsReady()
{
    return (state_ == State::Ready);
}

}
