#ifndef _THREAD_SINGLE_H__
#define _THREAD_SINGLE_H__

#include <thread>
#include <future>
#include <utility>
#include <functional>
#include <type_traits>
#include <typeinfo>
#include <memory>

#include "thread_info.h"

namespace thread {

class ThreadTemplateRet {

};

template < typename H, typename F, typename R>
class ThreadTemplate {
public:
    ThreadTemplate() {
    }
    ThreadTemplate(H* host, F func) {
        host_ = host;
        running_flag_= false;
        func_ = func;
        thread_ = NULL;
    }
    ThreadTemplate(ThreadTemplate& other) {
        host_ = other.host_;
        thread_info_ = other.thread_info_;
        func_ = other.func_;
        running_flag_ = other.running_flag_;
        thread_ = other.thread_;
        ret_ = other.ret_;
    }

    ~ThreadTemplate() {

    }

    const ThreadTemplate& operator=(const ThreadTemplate& other) {
        host_ = other.host_;
        thread_info_ = other.thread_info_;
        func_ = other.func_;
        running_flag_ = other.running_flag_;
        thread_ = other.thread_;
        ret_ = other.ret_;
        return *this;
    }

    template <typename ... Args>
    Handler* Run(Args&& ... args) {
        if (running_flag_) {
            return ThreadRet::THREAD_ERUNNING;
        }
        thread_ = std::make_shared<std::thread>(&ThreadTemplate::_run_main<Args ...>, this, std::forward<Args>(args)...);
        running_flag_ = true;
        return ThreadRet::SUCCESS;
    }

private:
    ThreadInfo* thread_info_;
private:
    H* host_;
    F func_;
    bool running_flag_;
    std::shared_ptr<std::thread> thread_;
    R ret_;

    template <typename ... Args>
    ThreadRet _run_main(Args&& ... args) {
        thread_info_ = ThreadInfo::getInstance();
        thread_info_->thread_ = thread_;
        thread_info_->Register2Process();

        ret_ = (host_->*func_)(std::forward<Args>(args)...);

        thread_info_->Unregister2Process();
        ThreadInfo::freeInstance();
        running_flag_ = false;
        thread_info_ = NULL;
        return ThreadRet::SUCCESS;
    }
};

template < typename F, typename R>
class ThreadTemplate<void, F, R> {
public:
    ThreadTemplate() {
    }
    ThreadTemplate(F func) {
        running_flag_= false;
        func_ = func;
        thread_ = NULL;
    }
    ThreadTemplate(ThreadTemplate& other) {
        thread_info_ = other.thread_info_;
        func_ = other.func_;
        running_flag_ = other.running_flag_;
        thread_ = other.thread_;
        ret_ = other.ret_;
    }

    ~ThreadTemplate() {

    }

    const ThreadTemplate& operator=(const ThreadTemplate& other) {
        thread_info_ = other.thread_info_;
        func_ = other.func_;
        running_flag_ = other.running_flag_;
        thread_ = other.thread_;
        ret_ = other.ret_;
        return *this;
    }

    bool IsRunning() {
        return running_flag_;
    }

    ThreadInfo* GetThreadInfo() {
        return thread_info_;
    }

    template <typename ... Args>
    ThreadRet Run(Args&& ... args) {
        if (running_flag_) {
            return ThreadRet::THREAD_ERUNNING;
        }
        thread_ = std::make_shared<std::thread>(&ThreadTemplate::_run_main<Args ...>, this, std::forward<Args>(args)...);
        running_flag_ = true;
        return ThreadRet::SUCCESS;
    }

    ThreadRet Join() {
        if (!running_flag_ || !thread_) {
            return ThreadRet::THREAD_ERUNNING;
        }
        thread_->join();
        return ThreadRet::SUCCESS;
    }

    ThreadRet Detach() {
        if (!running_flag_ || !thread_) {
            return ThreadRet::THREAD_ERUNNING;
        }
        thread_->detach();
        return ThreadRet::SUCCESS;
    }

    R& Return() {
        return ret_;
    }

private:
    ThreadInfo* thread_info_;
private:
    F func_;
    bool running_flag_;
    std::shared_ptr<std::thread> thread_;
    R ret_;

    template <typename ... Args>
    ThreadRet _run_main(Args&& ... args) {
        thread_info_ = ThreadInfo::getInstance();
        thread_info_->thread_ = thread_;
        thread_info_->Register2Process();

        ret_ = func_(std::forward<Args>(args)...);

        thread_info_->Unregister2Process();
        ThreadInfo::freeInstance();
        running_flag_ = false;
        thread_info_ = NULL;
        return ThreadRet::SUCCESS;
    }

};

#if 0
template < typename F >
class ThreadTemplate <F, ThreadTemplateRet> {
public:
    ThreadTemplate(F func) {
        running_flag_= false;
        func_ = func;
    }

    ~ThreadTemplate() {

    }

    ThreadInfo* GetThreadInfo() {
        return thread_info_;
    }

    template <typename ... Args>
    ThreadRet Run(Args&& ... args) {
        if (running_flag_) {
            return ThreadRet::THREAD_ERUNNING;
        }
        thread_ = std::thread(&ThreadTemplate::_run_main<Args ...>, this, std::forward<Args>(args)...);
        running_flag_ = true;
        return ThreadRet::SUCCESS;
    }

    ThreadRet Join() {
        thread_.join();
        return ThreadRet::SUCCESS;
    }

    void Detach() {
        thread_.detach();
    }

private:
    ThreadInfo* thread_info_;
private:
    F func_;
    bool running_flag_;
    std::thread thread_;

    template <typename ... Args>
    ThreadRet _run_main(Args&& ... args) {
        thread_info_ = ThreadInfo::getInstance();
//        thread_info_->thread_ = &thread_;
        thread_info_->Register2Process();

        func_(std::forward<Args>(args)...);

        thread_info_->Unregister2Process();
        ThreadInfo::freeInstance();
        return ThreadRet::SUCCESS;
    }

};
#endif

};

#endif
