#ifndef __THREAD_TEMPLATE_H__
#define __THREAD_TEMPLATE_H__

#include <functional>

#include "object.h"

//#include "process_handler.h"

#include "thread_return.h"
#include "thread_condition.h"
#include "thread_handler.h"

namespace infra::thread {

template <typename H, typename F>
class Template : virtual public ::infra::base::Object {
public:
    Template() : name_(""), cur_count_(0), max_count_(0) { }
    Template(H* host, F func) :
            host_(host), func_(func), name_("") {
        cur_count_ = 0;
        max_count_ = 0;
    }
    Template(std::string name, H* host, F func) :
            host_(host), func_(func), name_(name) {
        cur_count_ = 0;
        max_count_ = 0;
    }
    Template(const Template& other) {
        host_ = other.host_;
        func_ = other.func_;
        name_ = other.name_;
        cur_count_ = other.cur_count_;
        max_count_ = other.max_count_;
    }
    ~Template() { }

    Template& operator=(const Template& other) {
        host_ = other.host_;
        func_ = other.func_;
        name_ = other.name_;
        cur_count_ = other.cur_count_;
        max_count_ = other.max_count_;
        return *this;
    }

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
        Handler* handler;
        Condition cond;
        auto th = std::thread(_thread_main<H, F, Args ...>, host_, func_, name_, &handler, &cond, std::forward<Args>(args)...);
        cond.Wait();
        handler->thread_.swap(th);
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
    static Return _thread_main(HH* host, FF func, std::string name, Handler** phandler, Condition* cond, Args&& ... args) {
        Handler& handler = Handler::Instance();
        *phandler = &handler;
        handler.SetTid(ID::GetThreadID());
        handler.SetState(State::Running);
        if (handler.Register() != Return::SUCCESS) {
            handler.SetState(State::RegisterFail);
            return Return::THREAD_EREGISTER;
        }
        cond->NotifyAll();
        (host->*func)(std::forward<Args>(args)...);
        if (handler.Unregister() != Return::SUCCESS) {
            if (handler.GetState() != State::Running) {
                handler.SetState(State::UnregisterFail);
            }
            return Return::THREAD_EUNREGISTER;
        }
        if (handler.GetState() != State::Running) {
            handler.SetState(State::NormalExit);
        }
        return Return::SUCCESS;
    }
};

template <typename H, typename F> using TemplateMemFunction = Template<H, F>;
template <typename F> using TemplateFunction = Template<void, F>;

}

#endif
