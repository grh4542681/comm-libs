#ifndef __FSM_STATE_H__
#define __FSM_STATE_H__

#include <string>
#include <functional>

namespace fsm {

template <typename Host, typename StateType, typename = typename std::enable_if<std::is_enum<StateType>::value>::type>
class FsmState {
public:
    FsmState(StateType state) : state_(state) {

    }
    ~FsmState() {

    }

    StateType GetState() const {
        return state_;
    }

public:
    std::function<bool(Host*, FsmState&)> Enter = nullptr;
    std::function<bool(Host*, FsmState&)> Exit = nullptr;
    std::function<bool(Host*)> Update= nullptr;
private:
    StateType state_;
    std::string describe_;

    bool _default_enter(void* host, FsmState& prev_state) {
        FSM_DEBUG("No custmor enter function of state");
        return true;
    }
    bool _default_exit(void* host, FsmState& next_state) {
        FSM_DEBUG("No custmor enter function of state");
        return true;
    }
    bool _default_update(FsmState& curr_state) {
        FSM_DEBUG("No custmor enter function of state");
        return true;
    }
};

}

#endif
