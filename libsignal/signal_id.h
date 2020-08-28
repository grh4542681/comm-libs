#ifndef __SIGNAL_ID_H__
#define __SIGNAL_ID_H__

#include <string>

#include "object.h"

namespace infra::signal {

class Set;
class Process;

/**
* @brief - Linux signal Encapsulation.
*/
class ID : virtual public base::Object {
public:
    friend class Set;
    friend class Process;

    typedef int ID_t;
public:
    /**
    * @brief ID - Constructor.
    */
    ID();
    /**
    * @brief ID - Constructor.
    *
    * @param [sig] - Linux signal.
    */
    ID(int sig);
    /**
    * @brief ID - Copy constructor.
    *
    * @param [other] - Instance referance,
    */
    ID(const ID& other);
    ~ID();

    ID& operator= (const ID& other);
    bool operator< (const ID& other) const;

    ID_t GetInterID() const;
private:
    int sig_;   ///< Linux signal.
    std::string sig_name_;
};

}

#endif
