#ifndef __IO_SELECT_H__
#define __IO_SELECT_H__

#include "signal/process_signal_set.h"
#include "timer_time.h"

#include "io_return.h"
#include "io_fd.h"
#include "io_select_event.h"

#define SELECT_MAX_FD_SIZE (1024)

namespace io {

/**
* @brief - IO multiplexing basic class.
*/
class Select {
public:
    /**
    * @brief Select - Default constructor
    *
    * @param [item_size] - Maximum file descriptor size
    */
    Select(unsigned int item_size = SELECT_MAX_FD_SIZE);
    /**
    * @brief ~Select - Deconstructor
    */
    ~Select();

    /**
    * @brief AddEvent - Add a listen event
    *
    * @param [fd] - File descriptor
    * @param [events] - Events set
    *
    * @returns  IoRet
    */
    IoRet AddEvent(FD& fd, int events);
    /**
    * @brief AddEvent - Add a listen event
    *
    * @param [event] - SelectEvent
    *
    * @returns  IoRet
    */
    IoRet AddEvent(SelectEvent& event);
    /**
    * @brief ModEvent - Modify a listen event
    *
    * @param [fd] - File descriptor
    * @param [events] - New events set
    *
    * @returns  IoRet
    */
    IoRet ModEvent(FD& fd, int events);
    /**
    * @brief ModEvent - Modify a listen event
    *
    * @param [event] - New SelectEvent
    *
    * @returns  IoRet
    */
    IoRet ModEvent(SelectEvent& event);
    /**
    * @brief DelEvent - Delete a listen event
    *
    * @param [fd] - File descriptor
    *
    * @returns  IoRet
    */
    IoRet DelEvent(FD& fd);

    /**
    * @brief Listen - Blocking listening for all events
    *
    * @param [overtime] - Over time
    *
    * @returns  Vector of all triggered events
    */
    std::vector<SelectEvent> Listen(timer::Time* overtime);
    /**
    * @brief Listen - Blocking listening for all events
    *
    * @param [sigmask] - Signal shielding collection
    * @param [overtime] - Ovet time
    *
    * @returns  Vector of all triggered events
    */
    std::vector<SelectEvent> Listen(process::signal::ProcessSignalSet* sigmask, timer::Time* overtime);
private:
    bool init_flag_;            ///< Initialization flag
    int efd_;                   ///< epoll file descriptor
    unsigned int item_size_;    ///< Maximum file descriptor size
};

}

#endif
