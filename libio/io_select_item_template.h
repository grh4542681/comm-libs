#ifndef __IO_SELECT_ITEM_TEMPLATE_H__
#define __IO_SELECT_ITEM_TEMPLATE_H__

#include <string>
#include <functional>

#include "io_select_item.h"

namespace infra::io {

/**
* @brief - SelectItem adapter.
*
* @tparam [HOST] - Host class.
*/
template < typename HOST = void > class SelectItemTemplate : public io::SelectItem {
public:
    /**
    * @brief - SelectItem callback function type.
    */
    using CallbackFunc = std::function<IoRet(HOST*, SelectItemTemplate<HOST>*, int)>;
public:
    /**
    * @brief SelectItemTemplate - Default Constructor.
    */
    SelectItemTemplate() : io::SelectItem() { };
    /**
    * @brief SelectItemTemplate - Constructor.
    *
    * @param [host] - Host class pointer.
    * @param [fd] - File descriptor.
    * @param [callback] - Callback function.
    */
    SelectItemTemplate(HOST* host, const FD& fd, CallbackFunc callback = nullptr) : io::SelectItem(fd){
        host_instance_ = host;
        callback_ = callback;
    }

    /**
    * @brief SelectItemTemplate - Copy constructor.
    *
    * @param [other] - Other instance.
    */
    SelectItemTemplate(SelectItemTemplate& other) : io::SelectItem(other) {
        host_instance_ = other.host_instance_;
        callback_ = other.callback_;
    }

    /**
    * @brief ~SelectItemTemplate - Deconstructor.
    */
    virtual ~SelectItemTemplate() {}

    SelectItemTemplate& operator=(SelectItemTemplate& other) {
        io::SelectItem::operator=(other);
        host_instance_ = other.host_instance_;
        callback_ = other.callback_;
        return *this;
    }

    /**
    * @brief Callback - SelectItem callback adapter function.
    *                   Inherited from parent
    *
    * @param [events] - The set of events that make it fire.
    *
    * @returns  IoRet.
    */
    io::IoRet Callback(int events) {
        IoRet ret = IoRet::SUCCESS;
        if (callback_ != nullptr) {
            callback_(host_instance_, this, events);
        } else {
            _default_callback_func(host_instance_, this, events);
        }
        return io::IoRet::SUCCESS;
    }

    /**
    * @brief GetHost - Get host class pointer
    *
    * @returns  Host class pointer.
    */
    HOST* GetHost() {
        return host_instance_;
    }

    /**
    * @brief GetFd - Get File descriptor
    *
    * @tparam [T] - Real File descriptor type.
    *
    * @returns  Real File descriptor ref.
    */
    template < typename T > T& GetFd() {
        T& fd = dynamic_cast<T&>(event_.GetFd());
        return fd;
    }

    /**
    * @brief GetFdPointer - Get File descriptor pointer.
    *
    * @returns  File descriptor pointer.
    */
    FD* GetFdPointer() {
        return event_.GetFdPointer();
    }
private:
    HOST* host_instance_ = NULL;    ///< Host class instance pointer.
    CallbackFunc callback_;         ///< callback function.

private:
    /**
    * @brief _default_input_callback_func - Default input callback function.
    *
    * @returns  IoRet.
    */
    IoRet _default_input_callback_func() {
        printf("default i callback\n");
        return IoRet::SUCCESS;
    }
    /**
    * @brief _default_output_callback_func - Default output callback function.
    *
    * @returns  IoRet.
    */
    IoRet _default_output_callback_func() {
        printf("default o callback\n");
        return IoRet::SUCCESS;
    }
    /**
    * @brief _default_error_callback_func - Default error callback function.
    *
    * @returns  IoRet.
    */
    IoRet _default_error_callback_func() {
        printf("default e callback\n");
        return IoRet::SUCCESS;
    }
    /**
    * @brief _default_callback_func - Default callback function.
    *
    * @param [host] - Host class instance pointer.
    * @param [item] - SelectItem pointer being triggered.
    * @param [events] - The set of events that make it fire.
    *
    * @returns  IoRet.
    */
    IoRet _default_callback_func(HOST* host, SelectItem* item, int events) {
        IoRet ret = IoRet::SUCCESS;
        if (events & io::SelectEvent::Input) {
            ret = _default_input_callback_func();
            if (ret != IoRet::SUCCESS) {
                return io::IoRet::IO_EINPUTCB;
            }
            events |= ~io::SelectEvent::Input;
        }
        if (events & io::SelectEvent::Output) {
            ret = _default_output_callback_func();
            if (ret != IoRet::SUCCESS) {
                return io::IoRet::IO_EOUTPUTCB;
            }
            events |= ~io::SelectEvent::Output;
        }
        if (events & io::SelectEvent::Error) {
            ret = _default_error_callback_func();
            if (ret != IoRet::SUCCESS) {
                return io::IoRet::IO_EERRCB;
            }
            events |= ~io::SelectEvent::Error;
        }
        printf("default callback\n");
        return IoRet::SUCCESS;
    }
};

/**
* @brief - SelectItemTemplate Specialization with void.
*/
template < > class SelectItemTemplate<void> : public io::SelectItem {
public:
    /**
    * @brief - Callback function type.
    */
    using CallbackFunc = std::function<IoRet(SelectItemTemplate<void>*, int)>;
public:
    /**
    * @brief SelectItemTemplate - Default Constructor.
    */
    SelectItemTemplate() : io::SelectItem() { };
    /**
    * @brief SelectItemTemplate - Constructor.
    *
    * @param [host] - Host class pointer.
    * @param [fd] - File descriptor.
    * @param [callback] - Callback function.
    */
    SelectItemTemplate(const FD& fd, CallbackFunc callback = nullptr) : io::SelectItem(fd){
        callback_ = callback;
    }

    /**
    * @brief SelectItemTemplate - Copy constructor.
    *
    * @param [other] - Other instance.
    */
    SelectItemTemplate(SelectItemTemplate& other) : io::SelectItem(other) {
        callback_ = other.callback_;
    }

    /**
    * @brief ~SelectItemTemplate - Deconstructor.
    */
    virtual ~SelectItemTemplate() {}

    SelectItemTemplate& operator=(SelectItemTemplate& other) {
        io::SelectItem::operator=(other);
        callback_ = other.callback_;
        return *this;
    }

    /**
    * @brief Callback - SelectItem callback adapter function.
    *                   Inherited from parent
    *
    * @param [events] - The set of events that make it fire.
    *
    * @returns  IoRet.
    */
    io::IoRet Callback(int events) {
        IoRet ret = IoRet::SUCCESS;
        if (callback_ != nullptr) {
            callback_(this, events);
        } else {
            _default_callback_func(this, events);
        }
        return io::IoRet::SUCCESS;
    }

    /**
    * @brief GetFd - Get File descriptor
    *
    * @tparam [T] - Real File descriptor type.
    *
    * @returns  Real File descriptor ref.
    */
    template < typename T > T& GetFd() {
        T& fd = dynamic_cast<T&>(event_.GetFd());
        return fd;
    }

    /**
    * @brief GetFdPointer - Get File descriptor pointer.
    *
    * @returns  File descriptor pointer.
    */
    FD* GetFdPointer() {
        return event_.GetFdPointer();
    }
private:
    CallbackFunc callback_;         ///< callback function.

private:
    /**
    * @brief _default_input_callback_func - Default input callback function.
    *
    * @returns  IoRet.
    */
    IoRet _default_input_callback_func() {
        printf("default i callback\n");
        return IoRet::SUCCESS;
    }
    /**
    * @brief _default_output_callback_func - Default output callback function.
    *
    * @returns  IoRet.
    */
    IoRet _default_output_callback_func() {
        printf("default o callback\n");
        return IoRet::SUCCESS;
    }
    /**
    * @brief _default_error_callback_func - Default error callback function.
    *
    * @returns  IoRet.
    */
    IoRet _default_error_callback_func() {
        printf("default e callback\n");
        return IoRet::SUCCESS;
    }
    /**
    * @brief _default_callback_func - Default callback function.
    *
    * @param [host] - Host class instance pointer.
    * @param [item] - SelectItem pointer being triggered.
    * @param [events] - The set of events that make it fire.
    *
    * @returns  IoRet.
    */
    IoRet _default_callback_func(SelectItem* item, int events) {
        IoRet ret = IoRet::SUCCESS;
        if (events & io::SelectEvent::Input) {
            ret = _default_input_callback_func();
            if (ret != IoRet::SUCCESS) {
                return io::IoRet::IO_EINPUTCB;
            }
            events |= ~io::SelectEvent::Input;
        }
        if (events & io::SelectEvent::Output) {
            ret = _default_output_callback_func();
            if (ret != IoRet::SUCCESS) {
                return io::IoRet::IO_EOUTPUTCB;
            }
            events |= ~io::SelectEvent::Output;
        }
        if (events & io::SelectEvent::Error) {
            ret = _default_error_callback_func();
            if (ret != IoRet::SUCCESS) {
                return io::IoRet::IO_EERRCB;
            }
            events |= ~io::SelectEvent::Error;
        }
        printf("default callback\n");
        return IoRet::SUCCESS;
    }
};

}

#endif
