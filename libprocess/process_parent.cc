#include "process_parent.h"

namespace process {

ProcessParent::ProcessParent()
{
    name_.clear();
    init_flag_ = false;
}

ProcessParent::ProcessParent(std::string name, ProcessID& pid)
{
    name_ = name;
    pid_ = pid;
    init_flag_ = true;
}

ProcessParent::ProcessParent(std::string name, ProcessID&& pid)
{
    name_ = name;
    pid_ = pid;
    init_flag_ = true;
}

ProcessParent::ProcessParent(ProcessParent& other)
{
    pid_ = other.pid_;
    name_ = other.name_;

    fd_ = other.fd_;
    init_flag_ = other.init_flag_;
}

ProcessParent::~ProcessParent()
{
    if (init_flag_) {
        fd_.Close();
    }
}

ProcessID& ProcessParent::GetPid()
{
    return pid_;
}

std::string ProcessParent::GetName()
{
    return name_;
}

sock::SockFD& ProcessParent::GetFD()
{
    return fd_;
}

ProcessParent& ProcessParent::SetFD(sock::SockFD& fd)
{
    return SetFD(std::move(fd));
}

ProcessParent& ProcessParent::SetFD(sock::SockFD&& fd)
{
    fd_.SetFD(fd.GetFD());
    return *this;
}

ProcessRet ProcessParent::SetSendBlock(timer::Time* overtime)
{
    if (fd_.SetSendBlock(overtime) != sock::SockRet::SUCCESS) {
        PROCESS_ERROR("set Child sockfd in pair send block time failed");
        return ProcessRet::ERROR;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessParent::SetRecvBlock(timer::Time* overtime)
{
    if (fd_.SetRecvBlock(overtime) != sock::SockRet::SUCCESS) {
        PROCESS_ERROR("set Child sockfd in pair recv block time failed");
        return ProcessRet::ERROR;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessParent::SetNonBlock()
{
    if (fd_.SetNonBlock() != sock::SockRet::SUCCESS) {
        PROCESS_ERROR("set Child sockfd in pair non block failed");
        return ProcessRet::ERROR;
    }
    return ProcessRet::SUCCESS;
}

}
