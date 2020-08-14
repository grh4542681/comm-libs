#include "mempool.h"
#include "io_log.h"
#include "io_select_item.h"

namespace infra::io {

SelectItem::SelectItem()
{

}

SelectItem::SelectItem(const FD& fd)
{
    event_ = SelectEvent(fd);
    state_ = State::Normal;
}

SelectItem::SelectItem(const SelectItem& other)
{
    event_ = other.event_;
    state_ = other.state_;
}

const SelectItem& SelectItem::operator=(const SelectItem& other)
{
    event_ = other.event_;
    state_ = other.state_;
    return *this;
}

SelectItem::~SelectItem()
{
}

SelectItem::State SelectItem::GetState()
{
    return state_;
}

SelectItem& SelectItem::SetState(SelectItem::State state)
{
    state_ = state;
    return *this;
}

SelectEvent& SelectItem::GetSelectEvent()
{
    return event_;
}

}
