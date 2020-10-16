#include "thread_handler.h"

namespace infra::thread {

Handler::Handler() {
    name_   = "";
    thread_ = nullptr;
    state_  = State::Invalid;
    role_   = Role::Type::Normal;
}
Handler::~Handler() { }

Handler& Handler::SetThreadName(const char* name) {
    ScopedLock slock(mutex_);
    name_ = name;
    return *this;
}

Handler& Handler::SetTid(ID&& tid) {
    ScopedLock slock(mutex_);
    tid_ = tid;
    return *this;
}

Handler& Handler::SetState(State&& state) {
    ScopedLock slock(mutex_);
    state_ = state;
    return *this;
}

Handler& Handler::SetRole(Role&& role) {
    ScopedLock slock(mutex_);
    role_ = role;
    return *this;
}

std::string Handler::GetThreadName() {
    ScopedLock slock(mutex_);
    return name_;
}

ID Handler::GetTid() {
    ScopedLock slock(mutex_);
    return tid_;
}

State Handler::GetState() {
    ScopedLock slock(mutex_);
    return state_;
}

Role Handler::GetRole() {
    ScopedLock slock(mutex_);
    return role_;
}

Return Handler::Register() {
    return Return::SUCCESS;
}
Return Handler::Unregister() {
    return Return::SUCCESS;
}
Return Handler::Join() {
    return Return::SUCCESS;
}
Return Handler::Detach() {
    return Return::SUCCESS;
}

}
