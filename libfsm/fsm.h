#ifndef __FSM_H__
#define __FSM_H__

#include <string>
#include <map>

#include "fsm_log.h"
#include "fsm_return.h"

#include "fsm_trace.h"
#include "fsm_state.h"
#include "fsm_event.h"
#include "fsm_transition.h"

namespace fsm {

template <typename Host, typename EventType, typename StateType>
class Fsm {
public:
    Fsm(const StateType& state, Host* host = NULL) : host_(host), state_(state) { };
    ~Fsm() { };

    StateType& GetState() {
        return state_;
    }

    FsmRet AddEvent(const FsmEvent<Host, EventType>& event) {
        auto event_it = event_map_.find(event.GetEvent());
        if (event_it == event_map_.end()) {
            event_map_.insert({event.GetEvent(), event});
        } else {
            event_it->second = event;
        }
        return FsmRet::SUCCESS;
    }
    FsmRet DelEvent(const EventType& event) {
        auto event_it = event_map_.find(event);
        if (event_it != event_map_.end()) {
            for (auto trans_it : trans_map_) {
                if (std::get<EventType>(trans_it->first) == event) {
                    return FsmRet::FSM_EVENT_ETRANSEXIST;
                }
            }
            event_map_.erase(event_it);
        }
        return FsmRet::SUCCESS;
    }
    FsmEvent<Host, EventType>* GetEvent(const EventType& event) {
        auto event_it = event_map_.find(GetEvent());
        if (event_it != event_map_.end()) {
            return (&(event_it->second));
        } else {
            return NULL;
        }
    }

    FsmRet AddState(const FsmState<Host, StateType>& state) {
        auto state_it = state_map_.find(state.GetState());
        if (state_it == state_map_.end()) {
            state_map_.insert({state.GetState(), state});
        } else {
            state_it->second = state;
        }
        return FsmRet::SUCCESS;
    }
    FsmRet DelState(const StateType& state) {
        auto state_it = state_map_find(state);
        if (state_it != state_map_.end()) {
            for (auto trans_it : trans_map_) {
                if (std::get<StateType>(trans_it->first) == state) {
                    return FsmRet::FSM_EVENT_ETRANSEXIST;
                }
            }
            state_map_.erase(state_it);
        }
        return *this;
    }
    FsmState<Host, StateType>* GetState(const StateType& state) {
        auto state_it = state_map_.find(state);
        if (state_it != state_map_.end()) {
            return (&(state_it->second));
        } else {
            return NULL;
        }
    }

    FsmRet AddTransition(const FsmTransition<Host, EventType, StateType>& trans) {
        EventType event = trans.GetEvent();
        StateType state = trans.GetState();

        auto event_it = event_map_.find(event);
        if (event_it == event_map_.end()) {
            return FsmRet::FSM_EVENT_ENOTEXIST;
        }
        auto state_it = state_map_.find(state);
        if (state_it == state_map_.end()) {
            return FsmRet::FSM_STATE_ENOTEXIST;
        }

        auto trans_it = trans_map_.find(std::pair<EventType, StateType>(event, state));
        if (trans_it == trans_map_.end()) {
            trans_map_.insert({std::pair<EventType, StateType>(event, state), trans});
        } else {
            trans_it->second = trans;
        }
        return FsmRet::SUCCESS;
    }
    FsmRet DelTransition(const EventType& event, const StateType& state) {
        auto trans_it = trans_map_.find(std::pair<EventType, StateType>(event, state));
        if (trans_it == trans_map_.end()) {
            return FsmRet::SUCCESS;
        } else {
            trans_map_.erase(trans_it);
        }
        return FsmRet::SUCCESS;
    }
    Fsm& Clear() {
        event_map_.clear();
        state_map_.clear();
        trans_map_.clear();
        return *this;
    }

    FsmRet TriggerEvent(const EventType& event) {
        auto trans_it = trans_map_.find(std::pair<EventType, StateType>(event, state_));
        if (trans_it == trans_map_.end()) {
            return FsmRet::FSM_TRANS_ENOTEXIST;
        }

        auto state_it = state_map_.find(state_);
        if (state_it == state_map_.end()) {
            return FsmRet::FSM_STATE_ENOTEXIST;
        }
        auto next_state_it = state_map_.find(trans_it->second.GetNextState());
        if (next_state_it ==state_map_.end()) {
            return FsmRet::FSM_STATE_ENOTEXIST;
        }

        /// exec transition check function.
        if (trans_it->second.Check != nullptr) {
            if (!trans_it->second.Check(host_)) {
                //check error;
                return FsmRet::FSM_TRANS_ECHECK;
            }
        }

        /// if this transition no state change, exec update function.
        if (next_state_it->first == state_) {
            state_it->second.Update(host_);
        } else {
            /// exec current state exit function.
            if (state_it->second.Exit != nullptr) {
                if (!state_it->second.Exit(host_, next_state_it->second)) {
                    return FsmRet::FSM_STATE_EEXIT;
                }
            }
            /// exec next state enter function.
            if (next_state_it->second.Enter != nullptr) {
                if (!next_state_it->second.Enter(host_, state_it->second)) {
                    return FsmRet::FSM_STATE_EENTER;
                }
            }
        }
        /// exec transition complete function
        if (trans_it->second.Complete != nullptr) {
            if (!trans_it->second.Complete(host_)) {
                return FsmRet::FSM_TRANS_ECOMPLETE;
            }
        }
        /// change state
        state_ = next_state_it->first;
        return FsmRet::SUCCESS;
    }

    bool ChangeState(const StateType& state) {
        return true;
    }

    void ChangeStateForce(const StateType& state) {
        state_ = state;
    }
private:
    Host* host_;
    StateType state_;

    std::map<EventType, FsmEvent<Host, EventType>> event_map_;
    std::map<StateType, FsmState<Host, StateType>> state_map_;
    std::map<std::pair<EventType, StateType>, FsmTransition<Host, EventType, StateType>> trans_map_;
};

}

#endif
