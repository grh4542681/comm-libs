#ifndef __SIGNAL_ACTION_H__
#define __SIGNAL_ACTION_H__

#include "object.h"

#include "signal_set.h"

namespace infra::signal {

class Process;

/**
* @brief - Signal callback action
*/
class Action : virtual public base::Object {
public:
    friend class Process;
    /**
    * @brief  - Signal callback function type.
    *
    * @param [] - signal.
    */
    typedef void (*SignalCallback)(int);
    /**
    * @brief  - Signal callback function type.
    *
    * @param [] - signal
    * @param [siginfo_t] - siginfo
    * @param [] - ucontext_t
    *
    */
    typedef void (*SignalCallback2)(int, siginfo_t*, void*);
public:
    /**
    * @brief Action - Constructor.
    */
    Action();
    /**
    * @brief Action - Copy constructor.
    *
    * @param [other] - Instance ref.
    */
    Action(const Action& other);
    ~Action();

    Action& operator=(const Action& other);

    /**
    * @brief IsDefault - This instance is not the default behavior.
    *
    * @returns  bool.
    */
    bool IsDefault();
    /**
    * @brief SetCallback - Set signal callback function.
    *
    * @param [callback] - Callback function pointer.
    *
    * @returns  Self ref.
    */
    Action& SetCallback(SignalCallback callback);
    /**
    * @brief SetCallback - Set signal callback function.
    *
    * @param [callback] - Callback function pointer.
    *
    * @returns  Self ref.
    */
    Action& SetCallback(SignalCallback2 callback);
    /**
    * @brief SetMaskset - Set signal mask set.
    *
    * @param [set] - Signal mask set ref.
    *
    * @returns  Self ref.
    */
    Action& SetMaskset(Set& set);
    Action& SetMaskset(Set&& set);
private:
    Action(struct sigaction* action);
    struct sigaction action_;   ///< Linux sigaction.
};

}

#endif
