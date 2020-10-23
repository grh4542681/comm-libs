#ifndef __THREAD_ID_H__
#define __THREAD_ID_H__

#include <sys/types.h>
#include <iostream>

namespace infra::thread {

class ID {
public:
    friend std::ostream & operator<<(std::ostream &out, ID& tid);
public:
    typedef pid_t ID_t;
    typedef pthread_t PosixID_t;
public:
    ID();
    ID(ID_t tid);
    ID(ID& other);
    ID(const ID& other);
    ~ID();

    ID& operator=(ID other);
    bool operator==(ID& other);
    bool operator==(const ID& other);
    bool operator<(ID& other);
    bool operator<(const ID& other);
    bool operator>(ID& other);
    bool operator>(const ID& other);

    ID& SetInterID(ID_t tid);
    ID_t GetInterID() const;

    static ID GetThreadID();

private:
    ID_t tid_;
};

bool operator<(const ID& a, const ID& b);
std::ostream & operator<<(std::ostream &out, ID& tid);
std::ostream & operator<<(std::ostream &out, ID&& tid);

}

#endif
