#ifndef __IO_SELECT_ITEM_H__
#define __IO_SELECT_ITEM_H__

#include <sys/epoll.h>

#include "object.h"

#include "io_defines.h"
#include "io_fd.h"
#include "io_select_event.h"

namespace infra::io {


/**
* @brief - BaseItem that AutoSelect Listened.
*/
class SelectItem : virtual public base::Object {
public:
    /**
    * @brief - Item state.
    */
    enum class State {
        Normal,
        Add,
        Delete,
        Update,
    };
public:
    /**
    * @brief SelectItem - Default Constructor.
    */
    SelectItem();
    /**
    * @brief SelectItem - Constructor.
    *
    * @param [fd] - File descriptor.
    */
    SelectItem(const FD& fd);
    /**
    * @brief SelectItem - Copy constructor.
    *
    * @param [other] - Other instance.
    */
    SelectItem(const SelectItem& other);
    /**
    * @brief ~SelectItem - Deconstructor.
    */
    virtual ~SelectItem();

    const SelectItem& operator=(const SelectItem& other);

    /**
    * @brief GetState - Get item state.
    *
    * @returns  ItemState.
    */
    SelectItem::State GetState();
    /**
    * @brief SetState - Set item state.
    *
    * @param [state] - New state.
    *
    * @returns  Self.
    */
    SelectItem& SetState(State state);

    /**
    * @brief GetSelectEvent - Get Select event.
    *
    * @returns  SelectEvent.
    */
    SelectEvent& GetSelectEvent();

    /**
    * @brief Callback - Callback function.
    *                   Is called when this item's event is triggered.
    *
    * @param [events] - The set of events that make it fire.
    *
    * @returns  IoRet.
    */
    virtual IoRet Callback(int events){
        return IoRet::ESUBCLASS;
    }

protected:
    SelectEvent event_;             ///< Select event.
    State state_ = State::Normal;   ///< Item state.
};

}

#endif
