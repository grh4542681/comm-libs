#ifndef __SIGNAL_PROCESS_H__
#define __SIGNAL_PROCESS_H__

#include <map>

#include "object.h"

//#include "mutex/thread_rw_lock.h"
#include "signal_return.h"
#include "signal_id.h"
#include "signal_set.h"
#include "signal_action.h"
#include "signal_mask_type.h"

namespace infra::signal {

class Process : virtual public base::Object {
public:
//    thread::mutex::ThreadRWLock op_lock;                               ///< Muti thread lock
public:
    ~Process();

    Action GetAction(ID& sig);
    Action GetAction(ID&& sig);
    Set GetSignalMask();

    Return Register(ID& sig, Action& action);
    Return Register(ID&& sig, Action&& action);
    Return Register(ID& sig, Action& new_action, Action& old_action);
    Return Register(ID&& sig, Action&& new_action, Action&& old_action);
    Return UnRegister();
    Return UnRegister(ID& sig);
    Return UnRegister(ID&& sig);
    Return UnRegister(ID& sig, Action& old_action);
    Return UnRegister(ID&& sig, Action&& old_action);
    Return Revert();
    Return Revert(ID& sig);
    Return Revert(ID&& sig);
    
    Return Mask();
    Return Mask(Set& set);
    Return Mask(Set&& set);
    Return Mask(Set& new_set, Set& old_set);
    Return Mask(Set&& new_set, Set&& old_set);
    Return MaskReplace();
    Return MaskReplace(Set& set);
    Return MaskReplace(Set&& set);
    Return MaskReplace(Set& new_set, Set& old_set);
    Return MaskReplace(Set&& new_set, Set&& old_set);
    Return UnMask();
    Return UnMask(Set& set);
    Return UnMask(Set&& set);
    Return UnMask(Set& new_set, Set& old_set);
    Return UnMask(Set&& new_set, Set&& old_set);
    Return MaskRevert();

    static Return SendSignal(pid_t pid,ID& sig);
    static Process& Instance();
private:
    Process();

    std::map<ID, Action> register_map_;         ///< Current register signal map.
    std::map<ID, Action> last_register_map_;    ///< Last time register signal map.

    Set mask_set_;         ///< Current mask signal set.
    Set last_mask_set_;    ///< Last time mask signal set.

    static Process* pInstance;    ///< Signal instance pointer.

    Return _register_signal(ID&& sig, Action&& new_action, Action&& old_action);
    Return _mask_signal(SignalMaskType how, Set&& new_set, Set&& old_set);
};

}

#endif
