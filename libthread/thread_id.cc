#include <unistd.h>
#include <sys/syscall.h>

#include "thread_id.h"

#define gettidv1() syscall(__NR_gettid)

namespace infra::thread {

ID::ID()
{
    tid_ = 0;
}
ID::ID(ID_t tid)
{
    tid_ = tid;
}
ID::ID(ID& other)
{
    tid_ = other.tid_;
}
ID::ID(const ID& other)
{
    tid_ = other.tid_;
}
ID::~ID()
{

}

ID& ID::operator=(ID other)
{
    tid_ = other.tid_;
    return *this;
}

bool ID::operator==(ID& other)
{
    return (tid_ == other.tid_);
}

bool ID::operator==(const ID& other)
{
    return (tid_ == other.tid_);
}

bool ID::operator<(ID& other)
{
    return (tid_ < other.tid_);
}

bool ID::operator<(const ID& other)
{
    return (tid_ < other.tid_);
}

bool ID::operator>(ID& other)
{
    return (tid_ > other.tid_);
}

bool ID::operator>(const ID& other)
{
    return (tid_ > other.tid_);
}

ID& ID::SetInterID(ID_t tid)
{
    tid_ = tid;
    return *this;
}

ID::ID_t ID::GetInterID() const
{
    return tid_;
}

ID ID::GetThreadID()
{
    ID tid;
    tid.SetInterID((long int)gettidv1());
    return tid;
}

bool operator<(const ID& a, const ID& b)
{
    return (a.GetInterID() < b.GetInterID());
}

std::ostream & operator<<(std::ostream &out, ID& tid)
{
    out << tid.tid_;
    return out;
}

}
