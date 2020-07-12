#include <unistd.h>

#include "thread_id.h"

namespace thread {

ThreadID::ThreadID()
{
    tid_ = 0;
}
ThreadID::ThreadID(ThreadID_t tid)
{
    tid_ = tid;
}
ThreadID::ThreadID(ThreadID& other)
{
    tid_ = other.tid_;
}
ThreadID::ThreadID(const ThreadID& other)
{
    tid_ = other.tid_;
}
ThreadID::~ThreadID()
{

}

ThreadID& ThreadID::operator=(ThreadID other)
{
    tid_ = other.tid_;
    return *this;
}

bool ThreadID::operator==(ThreadID& other)
{
    return (tid_ == other.tid_);
}

bool ThreadID::operator==(const ThreadID& other)
{
    return (tid_ == other.tid_);
}

bool ThreadID::operator<(ThreadID& other)
{
    return (tid_ < other.tid_);
}

bool ThreadID::operator<(const ThreadID& other)
{
    return (tid_ < other.tid_);
}

bool ThreadID::operator>(ThreadID& other)
{
    return (tid_ > other.tid_);
}

bool ThreadID::operator>(const ThreadID& other)
{
    return (tid_ > other.tid_);
}

ThreadID& ThreadID::SetID(ThreadID_t tid)
{
    tid_ = tid;
    return *this;
}

ThreadID::ThreadID_t ThreadID::GetID() const
{
    return tid_;
}

ThreadID ThreadID::GetThreadID()
{
    ThreadID tid;
    tid.SetID(pthread_self());
    return tid;
}

bool operator<(const ThreadID& a, const ThreadID& b)
{
    return (a.GetID() < b.GetID());
}

std::ostream & operator<<(std::ostream &out, ThreadID& tid)
{
    out << tid.tid_;
    return out;
}

}
