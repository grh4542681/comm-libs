#ifndef __PROCESS_SIGNAL_H__
#define __PROCESS_SIGNAL_H__

#include <map>

#include "object.h"

//#include "mutex/thread_rw_lock.h"
#include "process_return.h"
#include "process_signal_id.h"
#include "process_signal_set.h"
#include "process_signal_action.h"
#include "process_signal_mask_type.h"

namespace infra::process {

class Signal : virtual public base::Object {
public:
//    thread::mutex::ThreadRWLock op_lock;                               ///< Muti thread lock
public:
    ~Signal();

    SignalAction GetSignalAction(SignalId& sig);
    SignalAction GetSignalAction(SignalId&& sig);
    SignalSet GetSignalMask();

    Return Register(SignalId& sig, SignalAction& action);
    Return Register(SignalId&& sig, SignalAction&& action);
    Return Register(SignalId& sig, SignalAction& new_action, SignalAction& old_action);
    Return Register(SignalId&& sig, SignalAction&& new_action, SignalAction&& old_action);
    Return UnRegister();
    Return UnRegister(SignalId& sig);
    Return UnRegister(SignalId&& sig);
    Return UnRegister(SignalId& sig, SignalAction& old_action);
    Return UnRegister(SignalId&& sig, SignalAction&& old_action);
    Return Revert();
    Return Revert(SignalId& sig);
    Return Revert(SignalId&& sig);
    
    Return Mask();
    Return Mask(SignalSet& set);
    Return Mask(SignalSet&& set);
    Return Mask(SignalSet& new_set, SignalSet& old_set);
    Return Mask(SignalSet&& new_set, SignalSet&& old_set);
    Return MaskReplace();
    Return MaskReplace(SignalSet& set);
    Return MaskReplace(SignalSet&& set);
    Return MaskReplace(SignalSet& new_set, SignalSet& old_set);
    Return MaskReplace(SignalSet&& new_set, SignalSet&& old_set);
    Return UnMask();
    Return UnMask(SignalSet& set);
    Return UnMask(SignalSet&& set);
    Return UnMask(SignalSet& new_set, SignalSet& old_set);
    Return UnMask(SignalSet&& new_set, SignalSet&& old_set);
    Return MaskRevert();

    static Return SendSignal(pid_t pid,SignalId& sig);
    static Signal& Instance();
private:
    Signal();

    std::map<SignalId, SignalAction> register_map_;         ///< Current register signal map.
    std::map<SignalId, SignalAction> last_register_map_;    ///< Last time register signal map.

    SignalSet mask_set_;         ///< Current mask signal set.
    SignalSet last_mask_set_;    ///< Last time mask signal set.

    static Signal* pInstance;    ///< Signal instance pointer.

    Return _register_signal(SignalId&& sig, SignalAction&& new_action, SignalAction&& old_action);
    Return _mask_signal(SignalMaskType how, SignalSet&& new_set, SignalSet&& old_set);
};

}

#endif
