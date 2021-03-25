#ifndef __PROCESS_ID_H__
#define __PROCESS_ID_H__

#include <sys/types.h>
#include <iostream>

namespace infra::process {

class ProcessID {
public:
    friend std::ostream & operator<<(std::ostream &out, ProcessID& pid);
public:
    typedef pid_t ProcessID_t;
public:
    ProcessID();
    ProcessID(ProcessID_t pid);
    ProcessID(ProcessID& other);
    ProcessID(const ProcessID& other);
    ~ProcessID();

    const ProcessID& operator=(ProcessID other);

    bool operator==(ProcessID& other);
    bool operator==(const ProcessID& other);
    bool operator<(ProcessID& other);
    bool operator<(const ProcessID& other);
    bool operator>(ProcessID& other);
    bool operator>(const ProcessID& other);

    ProcessID& SetID(ProcessID_t pid);
    ProcessID_t GetID() const;

    static ProcessID GetProcessID();
    static ProcessID GetProcessParentID();
private:
    ProcessID_t pid_;
};

bool operator<(const ProcessID& a, const ProcessID& b);
std::ostream & operator<<(std::ostream &out, ProcessID& pid);

}

#endif
