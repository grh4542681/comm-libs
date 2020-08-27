#ifndef __SIGNAL_SET_H__
#define __SIGNAL_SET_H__

#include <signal.h>

#include "object.h"

#include "signal_return.h"
#include "signal_id.h"

namespace infra::signal {

class Process;
class Action;

/**
* @brief - Signal set.
*/
class Set : virtual public base::Object {
public:
    friend class Action;
    friend class Process;
public:
    Set();
    Set(const Set& other);
    ~Set();

    const Set& operator= (const Set& other);

    bool IsEmpty();

    /**
    * @brief AddSig - Add a signal.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  Return.
    */
    Return AddSig(ID& sig);
    Return AddSig(ID&& sig);
    /**
    * @brief AddAll - Add all signal.
    *
    * @returns  Return.
    */
    Return AddAll();
    /**
    * @brief DelSig - Delete a signal from set.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  Return.
    */
    Return DelSig(ID& sig);
    Return DelSig(ID&& sig);
    /**
    * @brief DelAll - Clean set.
    *
    * @returns  Return.
    */
    Return DelAll();
    /**
    * @brief HasSig - Determine if the signal is in the map.
    *
    * @param [sig] - Signal ref.
    *
    * @returns  bool.
    */
    bool HasSig(ID& sig);
    bool HasSig(ID&& sig);

    sigset_t* GetSigset();

private:
    sigset_t set_;  ///< Linux signal set.
    bool empty_flag_;
};

}

#endif
