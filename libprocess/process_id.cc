#include <unistd.h>

#include "process_id.h"

namespace infra::process {

ID::ID()
{
    pid_ = 0;
}
ID::ID(ID_t pid)
{
    pid_ = pid;
}
ID::ID(ID& other)
{
    pid_ = other.pid_;
}
ID::ID(const ID& other)
{
    pid_ = other.pid_;
}
ID::~ID()
{

}

const ID& ID::operator=(ID other)
{
    pid_ = other.pid_;
    return *this;
}

bool ID::operator==(ID& other)
{
    return (pid_ == other.pid_);
}

bool ID::operator==(const ID& other)
{
    return (pid_ == other.pid_);
}

bool ID::operator<(ID& other)
{
    return (pid_ < other.pid_);
}

bool ID::operator<(const ID& other)
{
    return (pid_ < other.pid_);
}

bool ID::operator>(ID& other)
{
    return (pid_ > other.pid_);
}

bool ID::operator>(const ID& other)
{
    return (pid_ > other.pid_);
}

ID& ID::SetInterID(ID_t pid)
{
    pid_ = pid;
    return *this;
}

ID::ID_t ID::GetInterID() const
{
    return pid_;
}

ID ID::GetProcessID()
{
    ID pid;
    pid.SetInterID(getpid());
    return pid;
}

ID ID::GetParentProcessID()
{
    ID ppid;
    ppid.SetInterID(getppid());
    return ppid;
}

bool operator<(const ID& a, const ID& b)
{
    return (a.GetInterID() < b.GetInterID());
}

std::ostream & operator<<(std::ostream &out, ID& pid)
{
    out << pid.pid_;
    return out;
}

}
