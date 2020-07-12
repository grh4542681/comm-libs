#ifndef __IO_SELECT_EVENT_H__
#define __IO_SELECT_EVENT_H__

#include <utility>
#include <sys/epoll.h>

namespace io {

/**
* @brief - Select Event that SelectItem used.
*/
class SelectEvent {
public:
    const static int Input = EPOLLIN;           ///< Input event.
    const static int Output = EPOLLOUT;         ///< Output event.
    const static int Error = EPOLLERR;          ///< Error event.
    const static int Oneshot = EPOLLONESHOT;    ///< Only trigger once event.
public:
    /**
    * @brief SelectEvent - Default constructor.
    */
    SelectEvent() : events_(0) { }
    /**
    * @brief SelectEvent - Constructor
    *
    * @param [fd] - File descriptor.
    * @param [events] - Events set.
    */
    SelectEvent(const FD& fd, int events = 0) : events_(events) {
        fd_ = const_cast<FD&>(fd).Clone();
    }
    /**
    * @brief SelectEvent - Constructor
    *
    * @param [fd] - File descriptor.
    * @param [events] - Events set.
    */
    SelectEvent(const FD&& fd, int events = 0) : events_(events) {
        fd_ = const_cast<FD&>(fd).Clone();
    }
    /**
    * @brief SelectEvent - Copy constructor.
    *
    * @param [other] - Other instance.
    */
    SelectEvent(const SelectEvent& other) {
        if (other.fd_) {
            fd_ = other.fd_->Clone();
        }
        events_ = other.events_;
    }
    /**
    * @brief ~SelectEvent - Deconstructor.
    */
    ~SelectEvent() {
        if (fd_) {
            alloc_.Deallocate<FD>(fd_);
        }
    }

    SelectEvent& operator=(const SelectEvent& other) {
        if (fd_) {
            alloc_.Deallocate<FD>(fd_);
            fd_ = NULL;
        }
        if (other.fd_) {
            fd_ = other.fd_->Clone();
        }
        events_ = other.events_;
        return *this;
    }
    SelectEvent& operator|(int event) {
        events_ |= event;
        return *this;
    }
    SelectEvent& operator&(int event) {
        events_ &= event;
        return *this;
    }

    /**
    * @brief GetFd - Get File descriptor.
    *
    * @returns  File descriptor ref.
    */
    FD& GetFd() { return *fd_; };
    /**
    * @brief GetFdPointer - Get File descriptor pointer.
    *
    * @returns  File descriptor pointer.
    */
    FD* GetFdPointer() { return fd_; };
    int GetEvents() { return events_; };

    /**
    * @brief SetEvent - Set new events.
    *
    * @param [events] - New events set.
    *
    * @returns  Self.
    */
    SelectEvent& SetEvent(int events) {
        events_ = events;
        return *this;
    }

    /**
    * @brief HasInput - Whether to include input events.
    *
    * @returns  bool.
    */
    bool HasInput() { return (events_ & SelectEvent::Input); }
    /**
    * @brief HasOutput - Whether to include output events.
    *
    * @returns  bool.
    */
    bool HasOutput() { return (events_ & SelectEvent::Output); }
    /**
    * @brief HasError - Whether to include error events.
    *
    * @returns  bool.
    */
    bool HasError() { return (events_ & SelectEvent::Error); }
    /**
    * @brief HasOneshot - Whether to include oneshot events.
    *
    * @returns  bool.
    */
    bool HasOneshot() { return (events_ & SelectEvent::Oneshot); }

protected:
    FD* fd_ = NULL;     ///> File descriptor pointer.
    int events_ = 0;    ///> Listened events set.
    mempool::MempoolAlloctor alloc_;
};

}

#endif
