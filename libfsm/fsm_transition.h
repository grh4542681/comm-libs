#ifndef __FSM_TRANSITION_H__
#define __FSM_TRANSITION_H__

#include <string>

namespace fsm {

template <typename Host, typename EventType, typename StateType, 
            typename = typename std::enable_if<std::is_enum<EventType>::value && std::is_enum<StateType>::value>::type>
class FsmTransition {
public:
    FsmTransition(EventType event, StateType state, StateType to_state)
            : event_(event), state_(state), to_state_(to_state) {

    }
    ~FsmTransition() {

    }

    EventType GetEvent() const {
        return event_;
    }
    StateType GetState() const {
        return state_;
    }
    StateType GetNextState() const {
        return to_state_;
    }
public:
    std::function<bool(Host*)> Check = nullptr;
    std::function<bool(Host*)> Complete = nullptr;
private:
    EventType event_;
    StateType state_;
    StateType to_state_;
    std::string describe_;
};

}

#endif
