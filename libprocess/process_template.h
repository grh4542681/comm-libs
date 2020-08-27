#ifndef _PROCESS_TEMPLATE_H__
#define _PROCESS_TEMPLATE_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <string>
#include <functional>
#include <tuple>

#include "object.h"
#include "process_handler.h"

namespace infra::process {

template < typename F >
class Template : virtual public base::Object {
public:
    Template() {

    }
    Template(F child) {
        name_.clear();
        child_ = child;
    }

    Template(std::string name, F child) {
        name_ = name;
        child_ = child;
    }

    ~Template() {

    }

    void SetDeadCallback(void (*dead_callback)(int*)) {
        dead_callback_ = dead_callback;
    }

    template <typename ... Args>
    std::tuple<const Return, const ID> Run(Args&& ... args) {
        Log::Info("Starting create process.");
        //Create socket pair
        int fd_pair[2];
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd_pair)) {
            return {Return::ERROR, ID(0)};
        }

        pid_t pid = fork();
        if (pid < 0) {
            close(fd_pair[0]);
            close(fd_pair[1]);
            Log::Error("Fork error.");
            return {Return::PROCESS_EFORK, ID(0)};
        } else if (pid == 0) {
            //child
            Handler& handler = Handler::Instance();

            Parent parent(handler.GetName(), handler.GetPid());
            parent.SetFD(fd_pair[ParentSockFDIndex], true);
            close(fd_pair[ChildSockFDIndex]);

            handler.SetPid(ID::GetProcessID());
            handler.SetName(handler.GetName() + "_" + name_);
            handler.SetRealName(handler.GetName());
            handler.DelParent();
            handler.DelChild();
            handler.AddParent(parent);

            Log::Info("Execute child main function.");
            _run_main(std::forward<Args>(args)...);
            exit(0);
        } else {
            //parent
            Handler& handler = Handler::Instance();

            Child child(handler.GetName() + "_" + name_, ID(pid));
            child.SetFD(fd_pair[ChildSockFDIndex], true);
            close(fd_pair[ParentSockFDIndex]);
            child.SetDeadCallback(dead_callback_);

            Log::Info("Register child", pid, "into current process.");
            handler.AddChild(child);

            return {Return::SUCCESS, ID(pid)};
        }
    }
private:
    std::string name_;                  ///< Process Name.
    F child_;                           ///< Process main function.
    Child::ChildDeadCallback_t dead_callback_;       ///< Process dead callback for parent SIGCHLD.

    template <typename ... Args>
    Return _run_main(Args&& ... args) {
        child_(std::forward<Args>(args)...);
        return Return::SUCCESS;
    }
};

#if 0
/**
* @brief - Template template string type exception.
*/
template < >
class Template<std::string> {
public:
    /**
    * @brief Template - Constructor
    *
    * @param [child] - Process execute path.
    */
    Template(std::string child) {
        mempool_ = mempool::Mempool::getInstance();
        child_ = child;
    }

    /**
    * @brief Template - Constructor
    *
    * @param [name] - Process name.
    * @param [child] - Process execute path.
    */
    Template(std::string name, std::string child) {
        mempool_ = mempool::Mempool::getInstance();
        name_ = name;
        child_ = child;
    }

    ~Template() {

    }

    /**
    * @brief SetSigChldCallback - Set the callback if process dead parent process
    *                       will invoke the function.
    *
    * @param [sigchld_callback] - Callback function.
    */
    void SetSigChldCallback(void (*sigchld_callback)(int*)) {
        dead_callback_ = sigchld_callback;
    }

    /**
    * @brief Run - Start process.
    *
    * @returns  Return.
    */
    Return Run() {
        PROCESS_INFO("Starting create child process.");
        pid_t pid = fork();
        if (pid < 0) {
            PROCESS_ERROR("Fork error.");
            return Return::PROCESS_EFORK;
        } else if (pid == 0) {
            PROCESS_INFO("Execute child [%s].", child_.c_str());
            if (execl(child_.c_str(), child_.c_str(), 0) < 0) {
                PROCESS_ERROR("Exec bin [%s] errno [%d].", child_.c_str(), errno);
                exit(-1);
            }
        } else {
            ProcessInfo* parent = ProcessInfo::getInstance();
            ProcessInfo child_process_info;
            child_process_info.pid_ = pid;
            child_process_info.ppid_ = parent->pid_;

            PROCESS_INFO("Register child [%d] into current process.", pid);
            return parent->AddChildProcessInfo(child_process_info);
        }
        return Return::SUCCESS;
    }

    /**
    * @brief RunDaemon - Start daemon process.
    *
    * @returns  Return.
    */
    Return RunDaemon() {
        PROCESS_INFO("Starting create daemon process.");
        pid_t pid = fork();
        if (pid < 0) {
            PROCESS_ERROR("Fork error.");
            return Return::PROCESS_EFORK;
        } else if (pid == 0) {
            PROCESS_INFO("Daemon agent process [%d] starting daemon.", getpid());
            pid_t pid = fork();
            if (pid < 0) {
                PROCESS_ERROR("Daemon agent fork error.");
                exit(0);
            } else if (pid == 0) {
                PROCESS_INFO("Execute daemon [%s].", child_.c_str());
                if (execl(child_.c_str(), child_.c_str(), 0) < 0) {
                    PROCESS_ERROR("Exec daemon bin [%s] errno [%d].", child_.c_str(), errno);
                    exit(-1);
                }
            } else {
                PROCESS_INFO("Daemon agent process [%d] exit.", getpid());
                exit(0);
            }
        } else {
            ProcessInfo* parent = ProcessInfo::getInstance();
            ProcessInfo child_process_info;
            child_process_info.pid_ = pid;
            child_process_info.ppid_ = parent->pid_;

            PROCESS_INFO("Register child [%d] into current process.", pid);
            return parent->AddChildProcessInfo(child_process_info);
        }
    }

private:
    mempool::Mempool* mempool_;         ///< Mempool pointer.
    std::string name_;                  ///< Process Name.
    std::string child_;                 ///< Process execute path.
    void (*dead_callback_)(int*);    ///< Process dead callback for parent SIGCHLD.
};

#endif
};

#endif
