#ifndef __FSM_EVENT_H__
#define __FSM_EVENT_H__

#include <string>

namespace fsm {

template <typename Host, typename EventType, typename = typename std::enable_if<std::is_enum<EventType>::value>::type>
class FsmEvent {
public:
    FsmEvent(EventType event) : event_(event) {

    }
    ~FsmEvent() {

    }

    EventType GetEvent() const {
        return event_;
    }

private:
    std::string describe_;
    EventType event_;
};

}

#endif
