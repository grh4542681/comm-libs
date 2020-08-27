#ifndef __MODEL_EFSM_H__
#define __MODEL_EFSM_H__

#include <map>
#include <functional>

#include "model_log.h"
#include "model_return.h"

namespace infra::model {

template <typename H, typename EventType, typename StateType>
class Efsm {
public:

    class Event {
    public:
        Event(EventType event, std::string describe) : event_(event), describe_(describe) { }
        Event(const Event& other) {
            event_ = other.event_;
            describe_ = other.describe_;
        }
        ~Event() { }

        const Event& operator=(const Event& other) {
            event_ = other.event_;
            describe_ = other.describe_;
            return *this;
        }

        EventType GetEvent() const {
            return event_;
        }
        std::string GetDescribe() const {
            return describe_;
        }

        Event& SetProcessFunc(std::function<::infra::base::Return(H*)> func) {
            process = func;
            return *this;
        }

    public:
        std::function<::infra::base::Return(H*)> process = NULL;

    private:
        EventType event_;
        std::string describe_;
    };

    class State {
    public:
        State(StateType state, std::string describe) : state_(state), describe_(describe) { }
        State(const State& other) {
            enter = other.enter;
            exit = other.exit;
            update = other.update;
            state_ = other.state_;
            describe_ = other.describe_;
        }
        ~State() { }

        const State& operator=(const State& other) {
            enter = other.enter;
            exit = other.exit;
            update = other.update;
            state_ = other.state_;
            describe_ = other.describe_;
            return *this;
        }

        StateType GetState() const {
            return state_;
        }

        std::string GetDescribe() const {
            return describe_;
        }

        State& SetEnterFunc(std::function<::infra::base::Return(H*, State&)> func) {
            enter = func;
            return *this;
        }

        State& SetExitFunc(std::function<::infra::base::Return(H*, State&)> func) {
            exit = func;
            return *this;
        }

        State& SetUpdateFunc(std::function<::infra::base::Return(H*)> func) {
            update = func;
            return *this;
        }
    public:
        std::function<::infra::base::Return(H*, State&)> enter = NULL;
        std::function<::infra::base::Return(H*, State&)> exit = NULL;
        std::function<::infra::base::Return(H*)> update = NULL;
    private:
        StateType state_;
        std::string describe_;
    };

    class Transition {
    public:
        Transition(EventType event, StateType state, StateType to_state, std::string describe)
                : event_(event), state_(state), to_state_(to_state), describe_(describe) { }
        Transition(const Transition& other) {
            check = other.check;
            complete = other.complete;
            event_ = other.event_;
            state_ = other.state_;
            to_state_ = other.to_state_;
            describe_ = other.describe_;
        }
        ~Transition() { }

        const Transition& operator=(const Transition& other) {
            check = other.check;
            complete = other.complete;
            event_ = other.event_;
            state_ = other.state_;
            to_state_ = other.to_state_;
            describe_ = other.describe_;
            return *this;
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
        std::string GetDescribe() const {
            return describe_;
        }

        Transition& SetCheckFunc(std::function<int(H*)> func) {
            check = func;
            return *this;
        }
        Transition& SetCompleteFunc(std::function<int(H*)> func) {
            complete = func;
            return *this;
        }
    public:
        std::function<int(H*)> check = NULL;
        std::function<int(H*)> complete = NULL;
    private:
        EventType event_;
        StateType state_;
        StateType to_state_;
        std::string describe_;
    };

public:
    Efsm(const StateType& state, H* host = NULL) : host_(host), state_(state) { };
    Efsm(const Efsm& other) {
        host_ = NULL;
        state_ = other.state_;
        event_map_ = other.event_map_;
        state_map_ = other.state_map_;
        trans_map_ = other.trans_map_;
    }
    ~Efsm() { };

    const Efsm& operator=(const Efsm& other) {
        host_ = NULL;
        state_ = other.state_;
        event_map_ = other.event_map_;
        state_map_ = other.state_map_;
        trans_map_ = other.trans_map_;
        return *this;
    }

    Efsm& SetHost(H* host) {
        host_ = host;
        return *this;
    }

    StateType& GetState() {
        return state_;
    }

    Return AddEvent(const Event& event) {
        auto event_it = event_map_.find(event.GetEvent());
        if (event_it == event_map_.end()) {
            Log::Debug("Add new event [%s] in fsm", event.GetDescribe().c_str());
            auto ret = event_map_.insert({event.GetEvent(), event});
            if (std::get<1>(ret)) {
                return Return::SUCCESS;
            } else {
                Log::Error("Insert state map failed, return map end");
                return Return::ERROR;
            }
        } else {
            Log::Debug("ReSet event [%s] to event [%s]", event_it->second.GetDescribe().c_str(),
                            event.GetDescribe().c_str());
            event_it->second = event;
            return Return::SUCCESS;
        }
    }
    Return DelEvent(const EventType& event) {
        auto event_it = event_map_.find(event);
        if (event_it != event_map_.end()) {
            for (auto trans_it : trans_map_) {
                if (std::get<EventType>(trans_it->first) == event) {
                    return Return::MODEL_EFSM_EVENT_ETRANSEXIST;
                }
            }
            event_map_.erase(event_it);
        }
        return Return::SUCCESS;
    }
    Event* GetEvent(const EventType& event) {
        auto event_it = event_map_.find(GetEvent());
        if (event_it != event_map_.end()) {
            return (&(event_it->second));
        } else {
            return NULL;
        }
    }

   Return AddState(const State& state) {
        auto state_it = state_map_.find(state.GetState());
        if (state_it == state_map_.end()) {
            Log::Debug("Add new state [%s] in fsm", state.GetDescribe().c_str());
            auto ret = state_map_.insert({state.GetState(), state});
            if (std::get<1>(ret)) {
                return Return::SUCCESS;
            } else {
                Log::Error("Insert state map failed, return map end");
                return Return::ERROR;
            }
        } else {
            Log::Debug("ReSet state [%s] to state [%s]", state_it->second.GetDescribe().c_str(),
                            state.GetDescribe().c_str());
            state_it->second = state;
            return Return::SUCCESS;
        }
    }
    Return DelState(const StateType& state) {
        auto state_it = state_map_find(state);
        if (state_it != state_map_.end()) {
            for (auto trans_it : trans_map_) {
                if (std::get<StateType>(trans_it->first) == state) {
                    return Return::MODEL_EFSM_STATE_ETRANSEXIST;
                }
            }
            state_map_.erase(state_it);
        }
        return Return::SUCCESS;
    }
    State* GetState(const StateType& state) {
        auto state_it = state_map_.find(state);
        if (state_it != state_map_.end()) {
            return (&(state_it->second));
        } else {
            return NULL;
        }
    }

    Return AddTransition(const Transition& trans) {
        EventType event = trans.GetEvent();
        StateType state = trans.GetState();
        auto trans_it = trans_map_.find(std::pair<EventType, StateType>(event, state));
        if (trans_it == trans_map_.end()) {
            Log::Debug("Add new transition [%s] in fsm", trans.GetDescribe().c_str());
            auto ret = trans_map_.insert({std::pair<EventType, StateType>(event, state), trans});
            if (std::get<1>(ret)) {
                return Return::SUCCESS;
            } else {
                Log::Error("Insert trans map failed, return map end");
                return Return::ERROR;
            }
        } else {
            Log::Debug("ReSet trans [%s] to trans [%s]", trans_it->second.GetDescribe().c_str(),
                            trans.GetDescribe().c_str());
            trans_it->second = trans;
            return Return::SUCCESS;
        }
    }
    Return DelTransition(const EventType& event, const StateType& state) {
        auto trans_it = trans_map_.find(std::pair<EventType, StateType>(event, state));
        if (trans_it == trans_map_.end()) {
            return Return::SUCCESS;
        } else {
            trans_map_.erase(trans_it);
        }
        return Return::SUCCESS;
    }

    Transition* GetTransition(const EventType& event, const StateType& state) {
        auto trans_it = trans_map_.find(std::pair<EventType, StateType>(event, state));
        if (trans_it == trans_map_.end()) {
            return NULL;
        } else {
            return &(trans_it->second);
        }
    }

    Efsm& clear() {
        event_map_.clear();
        state_map_.clear();
        trans_map_.clear();
        return *this;
    }

    Return Trigger(const EventType& event) {
        Return rc;

        auto event_it = event_map_.find(event);
        if (event_it == event_map_.end()) {
            return Return::MODEL_EFSM_EVENT_ENOTEXIST;
        }

        auto trans_it = trans_map_.find(std::pair<EventType, StateType>(event, state_));
        if (trans_it == trans_map_.end()) {
            return Return::MODEL_EFSM_TRANS_ENOTEXIST;
        }

        auto state_it = state_map_.find(state_);
        if (state_it == state_map_.end()) {
            return Return::MODEL_EFSM_STATE_ENOTEXIST;
        }
        auto next_state_it = state_map_.find(trans_it->second.GetNextState());
        if (next_state_it ==state_map_.end()) {
            return Return::MODEL_EFSM_STATE_ENOTEXIST;
        }

        Log::Debug("Event %s, current state %s, next state %s",event_it->second.GetDescribe().c_str(),
                                state_it->second.GetDescribe().c_str(),
                                next_state_it->second.GetDescribe().c_str());

        /// exec transition check function.
        if (trans_it->second.check != NULL) {
            rc = trans_it->second.check(host_);
            if (rc != Return::SUCCESS) {
                //check error;
                return rc;
            }
        }

        /// exec event process function.
        if (event_it->second.process != NULL) {
            rc = event_it->second.process(host_);
            if (rc != Return::SUCCESS) {
                //process error;
                return rc;
            }
        }

        /// if this transition no state change, exec update function.
        if (next_state_it->first == state_) {
            if (state_it->second.exit != NULL) {
                rc = state_it->second.update(host_);
                if (rc != Return::SUCCESS) {
                    return rc;
                }
            }
        } else {
            /// exec current state exit function.
            if (state_it->second.exit != NULL) {
                rc = state_it->second.exit(host_, next_state_it->second);
                if (rc != Return::SUCCESS) {
                    return rc;
                }
            }
            /// exec next state enter function.
            if (next_state_it->second.enter != NULL) {
                rc = next_state_it->second.enter(host_, state_it->second);
                if (rc != Return::SUCCESS) {
                    return rc;
                }
            }
        }
        /// exec transition complete function
        if (trans_it->second.complete != NULL) {
            rc = trans_it->second.complete(host_);
            if (rc != Return::SUCCESS) {
                return rc;
            }
        }
        /// change state
        state_ = next_state_it->first;
        Log::Debug("state change to %s", next_state_it->second.GetDescribe().c_str());
        return Return::SUCCESS;
    }

    void changeStateForce(const StateType& state) {
        state_ = state;
    }
protected:
    H* host_;
    StateType state_;

    std::map<EventType, Event> event_map_;
    std::map<StateType, State> state_map_;
    std::map<std::pair<EventType, StateType>, Transition> trans_map_;
};

}

#endif
