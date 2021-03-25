#include "process_child.h"

namespace infra::process {

ProcessChild::ProcessChild()
{
    name_.clear();
    role_ |= ProcessRole::Child;
    state_ = ProcessState::Normal;

    dead_callback_ = NULL;
    init_flag_ = false;
}

ProcessChild::ProcessChild(std::string name, ProcessID&& pid)
{
    name_ = name;
    pid_ = pid;
    role_ |= ProcessRole::Child;
    state_ = ProcessState::Normal;

    dead_callback_ = NULL;
    init_flag_ = true;
}

ProcessChild::ProcessChild(ProcessChild& other)
{
    pid_ = other.pid_;
    name_ = other.name_;
    role_ = other.role_;
    state_ = other.state_;

    dead_callback_ = other.dead_callback_;
    fd_ = other.fd_;
    init_flag_ = other.init_flag_;
}

ProcessChild::~ProcessChild()
{

}

ProcessID& ProcessChild::GetPid()
{
    return pid_;
}

std::string ProcessChild::GetName()
{
    return name_;
}

ProcessRole& ProcessChild::GetRole()
{
    return role_;
}

ProcessState& ProcessChild::GetState()
{
    return state_;
}

void (*ProcessChild::GetDeadCallback())(int*)
{
    return dead_callback_;
}

sock::SockFD& ProcessChild::GetFD()
{
    return fd_;
}

ProcessChild& ProcessChild::SetState(ProcessState state)
{
    state_ = state;
    return *this;
}

ProcessChild& ProcessChild::SetDeadCallback(void (*dead_callback)(int*))
{
    dead_callback_ = dead_callback;
    return *this;
}

ProcessChild& ProcessChild::SetFD(sock::SockFD& fd)
{
    return SetFD(std::move(fd));
}
ProcessChild& ProcessChild::SetFD(sock::SockFD&& fd)
{
    fd_.SetFD(fd.GetFD());
    return *this;
}

ProcessRet ProcessChild::SetSendBlock(timer::Time* overtime)
{
    if (fd_.SetSendBlock(overtime) != sock::SockRet::SUCCESS) {
        PROCESS_ERROR("Set Child sockfd in pair send block time failed");
        return ProcessRet::ERROR;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessChild::SetRecvBlock(timer::Time* overtime)
{
    if (fd_.SetRecvBlock(overtime) != sock::SockRet::SUCCESS) {
        PROCESS_ERROR("Set Child sockfd in pair recv block time failed");
        return ProcessRet::ERROR;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessChild::SetNonBlock()
{
    if (fd_.SetNonBlock() != sock::SockRet::SUCCESS) {
        PROCESS_ERROR("Set Child sockfd in pair non block failed");
        return ProcessRet::ERROR;
    }
    return ProcessRet::SUCCESS;
}

}
