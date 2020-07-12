#ifndef __IO_EPOLL_FD__
#define __IO_EPOLL_FD__

#include "io_fd.h"
#include "io_select_event.h"

namespace io {

/**
* @brief - Epoll File descriptor
*/
class EpollFD : public FD{
public:
    /**
    * @brief EpollFD - Default constructor.
    */
    EpollFD();
    /**
    * @brief EpollFD - Constructor.
    *
    * @param [flags] - Just 0 or EPOLL_CLOEXEC.
    */
    EpollFD(int flags);
    /**
    * @brief EpollFD - Constructor.
    *
    * @param [fd] - Construct by an exist file descriptor.
    * @param [auto_close] - Auto close flag.
    */
    EpollFD(unsigned int fd, bool auto_close = false);
    /**
    * @brief EpollFD - Copy constructor.
    *
    * @param [other] - Other instance.
    */
    EpollFD(EpollFD& other);
    /**
    * @brief ~EpollFD - Deconstructor.
    */
    ~EpollFD();

    //Inherited from class FD.
    ret::Return SetFD(unsigned int fd, bool auto_close);
    ret::Return Dup(io::FD& new_fd);
    io::FD* Clone();
    void Close();
    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

    /**
    * @brief AddEvent - Add a listen event.
    *
    * @param [fd] - File descriptor.
    * @param [events] - Events set.
    *
    * @returns  IoRet.
    */
    IoRet AddEvent(FD& fd, int events);
    /**
    * @brief AddEvent - Add a listen event.
    *
    * @param [event] - SelectEvent.
    *
    * @returns  IoRet.
    */
    IoRet AddEvent(SelectEvent& event);
    /**
    * @brief ModEvent - Modify a listen event.
    *
    * @param [fd] - File descriptor.
    * @param [events] - New events set.
    *
    * @returns  IoRet
    */
    IoRet ModEvent(FD& fd, int events);
    /**
    * @brief ModEvent - Modify a listen event.
    *
    * @param [event] - New SelectEvent.
    *
    * @returns  IoRet.
    */
    IoRet ModEvent(SelectEvent& event);
    /**
    * @brief DelEvent - Delete a listen event.
    *
    * @param [fd] - File descriptor.
    *
    * @returns  IoRet.
    */
    IoRet DelEvent(FD& fd);
};

}

#endif
