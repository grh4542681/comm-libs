#ifndef __THREAD_TEMPLATE_H__
#define __THREAD_TEMPLATE_H__

#include <functional>

#include "object.h"

//#include "process_handler.h"

#include "thread_return.h"
#include "thread_handler.h"

namespace infra::thread {

template <typename H, typename F>
class Template : virtual public ::infra::base::Object {
public:
    Template(H* host, F func) {
        host_ = host;
        func_ = func;
        name_ = "";
        cur_count_ = 0;
        max_count_ = 0;
    }
    ~Template() { }

    Template<H, F>& SetName(std::string name) {
        name_ = name;
        return *this;
    }
    Template<H, F>& SetMaxCount(size_t max) {
        max_count_ = max;
        return *this;
    }

    std::string GetName(std::string name) {
        return name_;
    }
    size_t GetCurCount() {
        return cur_count_;
    }
    size_t GetMaxCount() {
        return max_count_;
    }

    template <typename ... Args>
    std::tuple<Return, Handler*> Run(Args&& ... args) {
        if (max_count_ != 0 && cur_count_ >= max_count_) {
            return {Return::THREAD_TEMPLATE_ECOUNTLIMIT, nullptr};
        }
        Handler* handler = new Handler();
        handler->name_ = name_;
        handler->state_ = State::Prepare;
        handler->thread_ = std::thread(_thread_main<H, F, Args ...>, host_, func_, handler, std::forward<Args>(args)...);
       std::cout << "---out [" <<  handler->thread_.get_id() <<"]"<< std::endl;
        cur_count_++;
        return {Return::SUCCESS, handler};
    }
protected:
    H* host_;
    F func_;
    std::string name_;
    size_t cur_count_;
    size_t max_count_;
    std::function<void(H*)> thread_prepare_;
    std::function<void(H*)> thread_exit_;

    template <typename HH, typename FF, typename ... Args>
    static Return _thread_main(HH* host, FF func, Handler* handler, Args&& ... args) {
        handler->SetTid(ID::GetThreadID());
       std::cout << "---in [" <<  ID::GetThreadID()  <<"]"<< std::endl;
        handler->SetState(State::Running);
        if (handler->Register() != Return::SUCCESS) {
            handler->SetState(State::RegisterFail);
            return Return::THREAD_EREGISTER;
        }
        (host->*func)(std::forward<Args>(args)...);
        if (handler->Unregister() != Return::SUCCESS) {
            if (handler->GetState() != State::Running) {
                handler->SetState(State::UnregisterFail);
            }
            return Return::THREAD_EUNREGISTER;
        }
        if (handler->GetState() != State::Running) {
            handler->SetState(State::NormalExit);
        }
        return Return::SUCCESS;
    }
};

template <typename H, typename F> using TemplateMemFunction = Template<H, F>;
template <typename F> using TemplateFunction = Template<void, F>;

}

#endif
