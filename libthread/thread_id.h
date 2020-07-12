#ifndef __THREAD_ID_H__
#define __THREAD_ID_H__

#include <sys/types.h>
#include <iostream>

namespace thread {

class ThreadID {
public:
    friend std::ostream & operator<<(std::ostream &out, ThreadID& tid);
public:
    typedef pid_t ThreadID_t;
public:
    ThreadID();
    ThreadID(ThreadID_t tid);
    ThreadID(ThreadID& other);
    ThreadID(const ThreadID& other);
    ~ThreadID();

    ThreadID& operator=(ThreadID other);
    bool operator==(ThreadID& other);
    bool operator==(const ThreadID& other);
    bool operator<(ThreadID& other);
    bool operator<(const ThreadID& other);
    bool operator>(ThreadID& other);
    bool operator>(const ThreadID& other);

    ThreadID& SetID(ThreadID_t tid);
    ThreadID_t GetID() const;

    static ThreadID GetThreadID();

private:
    ThreadID_t tid_;
};

bool operator<(const ThreadID& a, const ThreadID& b);
std::ostream & operator<<(std::ostream &out, ThreadID& pid);

}

#endif
