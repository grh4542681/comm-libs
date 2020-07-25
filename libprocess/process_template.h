/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-17 10:46
 * @file     : process_template.h
 * @brief    : Create process by template.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef _PROCESS_TEMPLATE_H__
#define _PROCESS_TEMPLATE_H__

#include <unistd.h>
#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <string>
#include <functional>
#include <tuple>

#include "mempool.h"

#include "process.h"
#include "process_info.h"
#include "signal/process_signal_ctrl.h"

namespace infra::process {

/**
* @brief - Create process.
*
* @tparam [F] - process main function
*/
template < typename F >
class Template {
public:
    Template() {

    }
    /**
    * @brief Template - Constructor
    *
    * @param [child] - process main function.
    */
    Template(F child) {
        mempool_ = mempool::Mempool::getInstance();
        name_.clear();
        child_ = child;
        auto_create_sockpair_ = false;
    }

    /**
    * @brief Template - Constructor
    *
    * @param [name] - Process name.
    * @param [child] - Process main function.
    */
    Template(std::string name, F child) {
        mempool_ = mempool::Mempool::getInstance();
        name_ = name;
        child_ = child;
        auto_create_sockpair_ = false;
    }

    ~Template() {

    }

    /**
    * @brief SetDeadCallback - Set the callback if process dead parent process
    *                       will invoke the function.
    *
    * @param [sigchld_callback] - Callback function.
    */
    void SetDeadCallback(void (*dead_callback)(int*)) {
        dead_callback_ = dead_callback;
    }

    /**
    * @brief Run - Start process.
    *
    * @tparam [Args] - Template Args of main function.
    * @param [args] - args.
    *
    * @returns  Return.
    */
    template <typename ... Args>
    std::tuple<const Return, const ProcessID> Run(Args&& ... args) {
        PROCESS_INFO("Starting create process.");
        //Create socket pair
        ipc::sock::SockPair pair;
        if (auto_create_sockpair_) {
            if (pair.Open() != ret::Return::SUCCESS) {
                PROCESS_ERROR("SockPair Open error.");
                return {Return::PROCESS_EFIFOPAIR, ProcessID(0)};
            }
            pair.SetAutoClose(false);
        }
        ProcessInfo* parent = ProcessInfo::getInstance();
        pid_t pid = fork();
        if (pid < 0) {
            if (auto_create_sockpair_) {
                pair.Close();
            }
            PROCESS_ERROR("Fork error.");
            return {Return::PROCESS_EFORK, ProcessID(0)};
        } else if (pid == 0) {
            //cache parent process data.
            ProcessParent parent_cache(parent->GetName(), parent->GetPid());
            if (auto_create_sockpair_) {
                pair[1].Close();
                parent_cache.SetFD(pair[0]);
            }

            char** raw_cmdline = NULL;
            unsigned int raw_cmdline_size = 0;
            parent->GetCmdLine(&raw_cmdline, &raw_cmdline_size);

            //destory parent mempool
            mempool::Mempool::freeInstance();
            ProcessInfo::setInstance(NULL);

            ProcessInfo* child = ProcessInfo::getInstance();
            child->SetName(name_);
            child->SetCmdLine(raw_cmdline, raw_cmdline_size);
            if (!name_.empty()) {
                Process::SetProcName(name_);
            }
            child->AddParentProcess(parent_cache);

            PROCESS_INFO("Execute child main function.");
            _run_main(std::forward<Args>(args)...);
            exit(0);
        } else {
            std::string child_name = name_;
            if (child_name.empty()) {
                child_name = parent->GetName() + "_" + std::to_string(pid);
            }
            ProcessID child_pid(pid);
            ProcessChild child(child_name, std::move(child_pid));
            if (auto_create_sockpair_) {
                pair[0].Close();
                child.SetFD(pair[1]);
            }
            child.SetDeadCallback(dead_callback_);
            child.GetRole().AddRole(ProcessRole::Child);
            child.SetState(ProcessState::Prepare);

            PROCESS_INFO("Register child [%d] into current process.", pid);
            parent->AddChildProcess(child);
            return {Return::SUCCESS, child_pid};
        }
        return {Return::SUCCESS, ProcessID(0)};
    }
#if 0
    /**
    * @brief RunDaemon - Start daemon process.
    *
    * @tparam [Args] - Template Args of main function.
    * @param [args] - args.
    *
    * @returns  Return.
    */
    template <typename ... Args>
    Return RunDaemon(Args&& ... args) {
        PROCESS_INFO("Starting create daemon process.");
        ProcessInfo* parent = ProcessInfo::getInstance();
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
                char** raw_cmdline = parent->raw_cmdline_;
                unsigned int raw_cmdline_size = parent->raw_cmdline_size_;

                //destory parent mempool
                mempool::Mempool::freeInstance();

                ProcessInfo::pInstance = NULL;
                ProcessInfo* child = ProcessInfo::getInstance();
                child->raw_cmdline_ = raw_cmdline;
                child->raw_cmdline_size_ = raw_cmdline_size;
                if (!name_.empty()) {
                    Process::SetProcName(name_);
                }
                Process::GetProcName(child->process_name_);

                PROCESS_INFO("Execute daemon main function.");
                _run_main(std::forward<Args>(args)...);
                exit(0);
            } else {
                PROCESS_INFO("Daemon agent process [%d] exit.", getpid());
                exit(0);
            }
        } else {
            return Return::SUCCESS;
        }
    }
#endif
private:
    mempool::Mempool* mempool_;         ///< Mempool pointer.
    std::string name_;                  ///< Process Name.
    F child_;                           ///< Process main function.
    bool auto_create_sockpair_;         ///< Auto create socketpair between process.
    void (*dead_callback_)(int*);       ///< Process dead callback for parent SIGCHLD.

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
