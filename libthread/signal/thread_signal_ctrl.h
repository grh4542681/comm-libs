#ifndef __THREAD_SIGNAL_CTRL_H__
#define __THREAD_SIGNAL_CTRL_H__

#include <map>

#include "mempool.h"
#include "mutex/thread_rw_lock.h"
#include "signal/process_signal.h"
#include "signal/process_signal_set.h"
#include "signal/process_signal_mask_type.h"
#include "timer_time.h"

#include "thread_return.h"

namespace thread::signal {

class ThreadSignalCtrl {
public:
    process::signal::ProcessSignalSet GetSignalMask();
    ThreadRet Mask();
    ThreadRet Mask(process::signal::ProcessSignalSet& set);
    ThreadRet Mask(process::signal::ProcessSignalSet&& set);
    /**
    * @brief Mask - Append mask new signal set.
    *
    * @param [new_set] - New signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  ThreadRet.
    */
    ThreadRet Mask(process::signal::ProcessSignalSet& new_set, process::signal::ProcessSignalSet& old_set);
    ThreadRet Mask(process::signal::ProcessSignalSet&& new_set, process::signal::ProcessSignalSet&& old_set);
    /**
    * @brief MaskReplace - Mask all signal
    *
    * @returns  ThreadRet.
    */
    ThreadRet MaskReplace();
    /**
    * @brief MaskReplace - Replace current mask signal by new mask signal set.
    *
    * @param [set] - New mask set.
    *
    * @returns  ThreadRet.
    */
    ThreadRet MaskReplace(process::signal::ProcessSignalSet& set);
    ThreadRet MaskReplace(process::signal::ProcessSignalSet&& set);
    /**
    * @brief MaskReplace - Replace current mask signal by new mask signal set.
    *
    * @param [new_set] - New signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  ThreadRet.
    */
    ThreadRet MaskReplace(process::signal::ProcessSignalSet& new_set, process::signal::ProcessSignalSet& old_set);
    ThreadRet MaskReplace(process::signal::ProcessSignalSet&& new_set, process::signal::ProcessSignalSet&& old_set);
    /**
    * @brief UnMask - Unmask all signal.
    *
    * @returns  ThreadRet.
    */
    ThreadRet UnMask();
    /**
    * @brief UnMask - Unmask signal set.
    *
    * @param [set] - Signal set.
    *
    * @returns  ThreadRet.
    */
    ThreadRet UnMask(process::signal::ProcessSignalSet& set);
    ThreadRet UnMask(process::signal::ProcessSignalSet&& set);
    /**
    * @brief UnMask - Unmask signal set.
    *
    * @param [new_set] - Signal set.
    * @param [old_set] - Old signal set.
    *
    * @returns  ThreadRet.
    */
    ThreadRet UnMask(process::signal::ProcessSignalSet& new_set, process::signal::ProcessSignalSet& old_set);
    ThreadRet UnMask(process::signal::ProcessSignalSet&& new_set, process::signal::ProcessSignalSet&& old_set);
    /**
    * @brief MaskRevert - Restore last time mask signal set.
    *
    * @returns  ThreadRet.
    */
    ThreadRet MaskRevert();

    ThreadRet SendSignal(pthread_t tid, process::signal::ProcessSignal& sig);
    static ThreadRet WaitSignal(timer::Time* overtime);

    /**
    * @brief getInstance - Get Signal ctrl class instance.
    *
    * @returns  Pointer of instance.
    */
    static ThreadSignalCtrl* getInstance();
private:
    ThreadSignalCtrl();
    ~ThreadSignalCtrl();

    mempool::Mempool* mempool_;                                         ///< Mempool pointer.

    process::signal::ProcessSignalSet mask_set_;         ///< Current mask signal set.
    process::signal::ProcessSignalSet last_mask_set_;    ///< Last time mask signal set.

    thread_local static ThreadSignalCtrl* pInstance;    ///< Signal instance pointer.

    ThreadRet _mask_signal(process::signal::SignalMaskType how, process::signal::ProcessSignalSet&& new_set, process::signal::ProcessSignalSet&& old_set);
};

}

#endif
