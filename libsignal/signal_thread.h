#ifndef __SIGNAL_THREAD__H__
#define __SIGNAL_THREAD__H__

#include <map>

//#include "mutex/thread_rw_lock.h"
#include "signal_return.h"
#include "signal_id.h"
#include "signal_set.h"
#include "signal_mask_type.h"
#include "timer_time.h"


namespace infra::signal {

class Thread {
public:
    Set GetSignalMask();
    Return Mask();
    Return Mask(Set& set);
    Return Mask(Set&& set);
    /**
    * @brief Mask - Append mask new signal set.
    *
    * @param [new_set] - New signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  Return.
    */
    Return Mask(Set& new_set, Set& old_set);
    Return Mask(Set&& new_set, Set&& old_set);
    /**
    * @brief MaskReplace - Mask all signal
    *
    * @returns  Return.
    */
    Return MaskReplace();
    /**
    * @brief MaskReplace - Replace current mask signal by new mask signal set.
    *
    * @param [set] - New mask set.
    *
    * @returns  Return.
    */
    Return MaskReplace(Set& set);
    Return MaskReplace(Set&& set);
    /**
    * @brief MaskReplace - Replace current mask signal by new mask signal set.
    *
    * @param [new_set] - New signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  Return.
    */
    Return MaskReplace(Set& new_set, Set& old_set);
    Return MaskReplace(Set&& new_set, Set&& old_set);
    /**
    * @brief UnMask - Unmask all signal.
    *
    * @returns  Return.
    */
    Return UnMask();
    /**
    * @brief UnMask - Unmask signal set.
    *
    * @param [set] - Signal set.
    *
    * @returns  Return.
    */
    Return UnMask(Set& set);
    Return UnMask(Set&& set);
    /**
    * @brief UnMask - Unmask signal set.
    *
    * @param [new_set] - Signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  Return.
    */
    Return UnMask(Set& new_set, Set& old_set);
    Return UnMask(Set&& new_set, Set&& old_set);
    /**
    * @brief MaskRevert - Restore last time mask signal set.
    *
    * @returns  Return.
    */
    Return MaskRevert();

    Return SendSignal(pthread_t tid, ID& sig);
    static Return WaitSignal(timer::Time* overtime);

    /**
    * @brief getInstance - Get Signal ctrl class instance.
    *
    * @returns  Pointer of instance.
    */
    static Thread* Instance();
private:
    Thread();
    ~Thread();                                        ///< Mempool pointer.

    Set mask_set_;         ///< Current mask signal set.
    Set last_mask_set_;    ///< Last time mask signal set.

    thread_local static Thread* pInstance;    ///< Signal instance pointer.

    Return _mask_signal(SignalMaskType how, Set&& new_set, Set&& old_set);
};

}

#endif
