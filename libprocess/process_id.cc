#include <unistd.h>

#include "process_id.h"

namespace process {

ProcessID::ProcessID()
{
    pid_ = 0;
}
ProcessID::ProcessID(ProcessID_t pid)
{
    pid_ = pid;
}
ProcessID::ProcessID(ProcessID& other)
{
    pid_ = other.pid_;
}
ProcessID::ProcessID(const ProcessID& other)
{
    pid_ = other.pid_;
}
ProcessID::~ProcessID()
{

}

const ProcessID& ProcessID::operator=(ProcessID other)
{
    pid_ = other.pid_;
    return *this;
}

bool ProcessID::operator==(ProcessID& other)
{
    return (pid_ == other.pid_);
}

bool ProcessID::operator==(const ProcessID& other)
{
    return (pid_ == other.pid_);
}

bool ProcessID::operator<(ProcessID& other)
{
    return (pid_ < other.pid_);
}

bool ProcessID::operator<(const ProcessID& other)
{
    return (pid_ < other.pid_);
}

bool ProcessID::operator>(ProcessID& other)
{
    return (pid_ > other.pid_);
}

bool ProcessID::operator>(const ProcessID& other)
{
    return (pid_ > other.pid_);
}

ProcessID& ProcessID::SetID(ProcessID_t pid)
{
    pid_ = pid;
    return *this;
}

ProcessID::ProcessID_t ProcessID::GetID() const
{
    return pid_;
}

ProcessID ProcessID::GetProcessID()
{
    ProcessID pid;
    pid.SetID(getpid());
    return pid;
}

ProcessID ProcessID::GetProcessParentID()
{
    ProcessID ppid;
    ppid.SetID(getppid());
    return ppid;
}

bool operator<(const ProcessID& a, const ProcessID& b)
{
    return (a.GetID() < b.GetID());
}

std::ostream & operator<<(std::ostream &out, ProcessID& pid)
{
    out << pid.pid_;
    return out;
}

}
