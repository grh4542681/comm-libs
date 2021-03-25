#ifndef __IO_AUTO_SELECT_H__
#define __IO_AUTO_SELECT_H__

#include <sys/epoll.h>

#include "object.h"
#include "thread_template.h"
#include "thread_mutex.h"
#include "signal_set.h"
#include "timer_time.h"
#include "timer_fd.h"

#include "io_epoll_fd.h"
#include "io_select_item.h"

namespace infra::io {

/**
* @brief - IO multiplexing automatic processing class.
*/
class AutoSelect : virtual public base::Object {
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
    * @returns  Return.
    */
    template <typename T, typename ... Args> Return AddSelectItem(Args&& ... args)
    {
        SelectItem* item = alloc_.Allocate<T>(std::forward<Args>(args)...);
        if (!item) {
            return Return::EMALLOC;
        }
        std::pair<std::map<int, SelectItem*>::iterator, bool> map_ret = select_item_map_.insert({item->GetSelectEvent().GetFd().GetFD(), item});
        if (map_ret.second == false) {
            alloc_.Deallocate<T>(dynamic_cast<T*>(item));
            return Return::EMAP;
        }
        Return ret = fd_.AddEvent(item->GetSelectEvent());
        if (ret != Return::SUCCESS) {
            select_item_map_.erase(item->GetSelectEvent().GetFd().GetFD());
            alloc_.Deallocate<T>(dynamic_cast<T*>(item));
            return ret;
        }
        return ret;
    }
    Return DelSelectItem(FD& fd);
    Return ModSelectItem();
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
    * @returns  Return.
    */
    Return Listen(timer::Time overtime);
    /**
    * @brief Listen - Start listening.
    *                 It will occupy the current thread and will not return.
    *
    * @param [sigmask] - Signal masking set during monitoring.
    * @param [overtime] - Over time.
    *                     if non-blocking, This parameter will be ignored.
    *
    * @returns  Return.
    */
    Return Listen(signal::Set sigmask, timer::Time overtime);
    /**
    * @brief ListenThread - Start listening.
    *                       It will create a new thread, does not take up the current thread.
    *
    * @param [overtime] - Over time.
    *                     if non-blocking, This parameter will be ignored.
    *
    * @returns  
    */
    Return ListenThread(timer::Time overtime);
    /**
    * @brief ListenThread - Start listening.
    *                       It will create a new thread, does not take up the current thread.
    *
    * @param [sigmask] - Signal masking set during monitoring.
    * @param [overtime] - Over time.
    *                     if non-blocking, This parameter will be ignored.
    *
    * @returns  Return.
    */
    Return ListenThread(signal::Set sigmask, timer::Time overtime);

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
    static Return _select_listener_thread_handler(AutoSelect* instance, signal::Set sigmask, timer::Time overtime);

private:
    bool init_flag_ = false;        ///< Initialization flag.
    bool nonblock_flag_ = false;    ///< Non-blocking flag.
    EpollFD fd_;                    ///< epoll file descriptor.

    thread::Mutex mutex_;          ///< Thread mutex lock.
    std::map<int, SelectItem*> select_item_map_;    ///< SelectItem map.

    thread::ThreadTemplate<void, decltype(&_select_listener_thread_handler), Return> listener_thread_; ///< Listener thread instance.

    Return _select_item_traversal();
};

}

#endif
