#ifndef __IO_AUTO_SELECT_H__
#define __IO_AUTO_SELECT_H__

#include <sys/epoll.h>

#include "mempool.h"
#include "thread_template.h"
#include "mutex/thread_mutex_lock.h"
#include "signal/process_signal_set.h"
#include "timer_time.h"
#include "timer_fd.h"

#include "io_epoll_fd.h"
#include "io_select_item.h"

namespace infra::io {

/**
* @brief - IO multiplexing automatic processing class.
*/
class AutoSelect {
public:
    /**
    * @brief AutoSelect - Default Constructor.
    */
    AutoSelect();
    /**
    * @brief ~AutoSelect - Deconstructor.
    */
    ~AutoSelect();

    /**
    * @brief AddSelectItem - Add an item to be monitored.
    *
    * @tparam [T] - SelectItem or a subclass of SelectItem.
    * @tparam [Args] - Variable length parameter type.
    * @param [args] - Parameters required for T instantiation.
    *
    * @returns  IoRet.
    */
    template <typename T, typename ... Args> IoRet AddSelectItem(Args&& ... args)
    {
        SelectItem* item = alloc_.Allocate<T>(std::forward<Args>(args)...);
        if (!item) {
            return IoRet::EMALLOC;
        }
        std::pair<std::map<int, SelectItem*>::iterator, bool> map_ret = select_item_map_.insert({item->GetSelectEvent().GetFd().GetFD(), item});
        if (map_ret.second == false) {
            alloc_.Deallocate<T>(dynamic_cast<T*>(item));
            return IoRet::EMAP;
        }
        IoRet ret = fd_.AddEvent(item->GetSelectEvent());
        if (ret != IoRet::SUCCESS) {
            select_item_map_.erase(item->GetSelectEvent().GetFd().GetFD());
            alloc_.Deallocate<T>(dynamic_cast<T*>(item));
            return ret;
        }
        return ret;
    }
    IoRet DelSelectItem(FD& fd);
    IoRet ModSelectItem();
    const SelectItem* GetSelectItem(FD& fd);
    /**
    * @brief HasSelectItem - Whether to include item.
    *
    * @param [fd] - File descriptor.
    *
    * @returns  bool.
    */
    bool HasSelectItem(FD& fd);

    /**
    * @brief GetListenrThread - Get listener thread instance.
    *
    * @returns  thread::ThreadTemplate instance ref.
    */
    auto& GetListenrThread();
    /**
    * @brief GetNonblock - Get Blocking or non-blocking listening.
    *
    * @returns  bool.
    */
    bool GetNonblock();

    /**
    * @brief Listen - Start listening.
    *                 It will occupy the current thread and will not return.
    *
    * @param [overtime] - Over time.
    *                     if non-blocking, This parameter will be ignored.
    *
    * @returns  IoRet.
    */
    IoRet Listen(timer::Time overtime);
    /**
    * @brief Listen - Start listening.
    *                 It will occupy the current thread and will not return.
    *
    * @param [sigmask] - Signal masking set during monitoring.
    * @param [overtime] - Over time.
    *                     if non-blocking, This parameter will be ignored.
    *
    * @returns  IoRet.
    */
    IoRet Listen(process::signal::ProcessSignalSet sigmask, timer::Time overtime);
    /**
    * @brief ListenThread - Start listening.
    *                       It will create a new thread, does not take up the current thread.
    *
    * @param [overtime] - Over time.
    *                     if non-blocking, This parameter will be ignored.
    *
    * @returns  
    */
    IoRet ListenThread(timer::Time overtime);
    /**
    * @brief ListenThread - Start listening.
    *                       It will create a new thread, does not take up the current thread.
    *
    * @param [sigmask] - Signal masking set during monitoring.
    * @param [overtime] - Over time.
    *                     if non-blocking, This parameter will be ignored.
    *
    * @returns  IoRet.
    */
    IoRet ListenThread(process::signal::ProcessSignalSet sigmask, timer::Time overtime);

private:
    /**
    * @brief _select_listener_thread_handler - Main function for automatic monitoring processing.
    *
    * @param [instance] - AutoSelect instance pointer.
    * @param [sigmask] - Signal masking set during monitoring.
    * @param [overtime] - Over time.
    *                     if non-blocking, This parameter will be ignored.
    *
    * @returns  
    */
    static IoRet _select_listener_thread_handler(AutoSelect* instance, process::signal::ProcessSignalSet sigmask, timer::Time overtime);

private:
    bool init_flag_ = false;        ///< Initialization flag.
    bool nonblock_flag_ = false;    ///< Non-blocking flag.
    mempool::MempoolAlloctor alloc_;
    EpollFD fd_;                    ///< epoll file descriptor.

    thread::mutex::ThreadMutexLock mutex_;          ///< Thread mutex lock.
    std::map<int, SelectItem*> select_item_map_;    ///< SelectItem map.

    thread::ThreadTemplate<void, decltype(&_select_listener_thread_handler), IoRet> listener_thread_; ///< Listener thread instance.

    IoRet _select_item_traversal();
};

}

#endif
