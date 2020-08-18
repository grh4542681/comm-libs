#ifndef __PROCESS_ROLE_H__
#define __PROCESS_ROLE_H__

#include "object.h"

namespace infra::process {

class Role : virtual public base::Object {
public:
    Role();
    Role(Role&);
    ~Role();

    Role& Clean();

    Role operator~();

    Role& operator=(Role& a);
    Role& operator=(Role&& a);
    Role operator|(Role& a);
    Role operator|(Role&& a);
    Role& operator|=(Role& a);
    Role& operator|=(Role&& a);
    Role operator&(Role& a);
    Role operator&(Role&& a);
    Role& operator&=(Role& a);
    Role& operator&=(Role&& a);

    Role& operator=(unsigned int& a);
    Role& operator=(unsigned int&& a);
    Role operator|(unsigned int& a);
    Role operator|(unsigned int&& a);
    Role& operator|=(unsigned int& a);
    Role& operator|=(unsigned int&& a);
    Role operator&(unsigned int& a);
    Role operator&(unsigned int&& a);
    Role& operator&=(unsigned int& a);
    Role& operator&=(unsigned int&& a);

    Role& AddRole(unsigned int role);
    Role& DelRole(unsigned int role);
    bool HasRole(unsigned int role);
public:
    static unsigned int Normal;
    static unsigned int Parent;
    static unsigned int Child;
    static unsigned int PoolWorker;
    static unsigned int PoolKeeper;
private:
    Role(unsigned int role);
    unsigned int role_;
};

}

#endif
