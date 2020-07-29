#ifndef __PROCESS_ID_H__
#define __PROCESS_ID_H__

#include <sys/types.h>
#include <iostream>

namespace infra::process {

class ID {
public:
    friend std::ostream & operator<<(std::ostream &out, ID& pid);
public:
    typedef pid_t ID_t;
public:
    ID();
    ID(ID_t pid);
    ID(ID& other);
    ID(const ID& other);
    ~ID();

    const ID& operator=(ID other);

    bool operator==(ID& other);
    bool operator==(const ID& other);
    bool operator<(ID& other);
    bool operator<(const ID& other);
    bool operator>(ID& other);
    bool operator>(const ID& other);

    ID& SetInterID(ID_t pid);
    ID_t GetInterID() const;

    static ID GetProcessID();
    static ID GetParentProcessID();
private:
    ID_t pid_;
};

bool operator<(const ID& a, const ID& b);
std::ostream & operator<<(std::ostream &out, ID& pid);

}

#endif
