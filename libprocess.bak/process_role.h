#ifndef __PROCESS_ROLE_H__
#define __PROCESS_ROLE_H__

namespace infra::process {

class ProcessRole {
public:
    ProcessRole();
    ProcessRole(ProcessRole&);
    ~ProcessRole();

    ProcessRole& Clean();

    ProcessRole operator~();

    ProcessRole& operator=(ProcessRole& a);
    ProcessRole& operator=(ProcessRole&& a);
    ProcessRole operator|(ProcessRole& a);
    ProcessRole operator|(ProcessRole&& a);
    ProcessRole& operator|=(ProcessRole& a);
    ProcessRole& operator|=(ProcessRole&& a);
    ProcessRole operator&(ProcessRole& a);
    ProcessRole operator&(ProcessRole&& a);
    ProcessRole& operator&=(ProcessRole& a);
    ProcessRole& operator&=(ProcessRole&& a);

    ProcessRole& operator=(unsigned int& a);
    ProcessRole& operator=(unsigned int&& a);
    ProcessRole operator|(unsigned int& a);
    ProcessRole operator|(unsigned int&& a);
    ProcessRole& operator|=(unsigned int& a);
    ProcessRole& operator|=(unsigned int&& a);
    ProcessRole operator&(unsigned int& a);
    ProcessRole operator&(unsigned int&& a);
    ProcessRole& operator&=(unsigned int& a);
    ProcessRole& operator&=(unsigned int&& a);

    ProcessRole& AddRole(unsigned int role);
    ProcessRole& DelRole(unsigned int role);
    bool HasRole(unsigned int role);
public:
    static unsigned int Normal;
    static unsigned int Parent;
    static unsigned int Child;
    static unsigned int PoolWorker;
    static unsigned int PoolKeeper;
private:
    ProcessRole(unsigned int role);
    unsigned int role_;
};

}

#endif
