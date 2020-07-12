#ifndef __PROCESS_POOL_WORKER_TEMPLATE_H__
#define __PROCESS_POOL_WORKER_TEMPLATE_H__

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

namespace process::pool {

template < typename F >
class ProcessPoolWorkerTemplate {
public:
    ProcessPoolWorkerTemplate() {

    }
    /**
    * @brief ProcessPoolWorkerTemplate - Constructor
    *
    * @param [child] - process main function.
    */
    ProcessPoolWorkerTemplate(std::string name, F child) {
        mempool_ = mempool::Mempool::getInstance();
        name_.clear();
        config_filename_.clear();
        child_ = child;
    }

    /**
    * @brief ProcessPoolWorkerTemplate - Constructor
    *
    * @param [name] - Process name.
    * @param [child] - Process main function.
    */
    ProcessPoolWorkerTemplate(std::string name, std::string config_filename, F child) {
        mempool_ = mempool::Mempool::getInstance();
        name_ = name;
        config_filename_ = config_filename;
        child_ = child;
    }

    ~ProcessPoolWorkerTemplate() {

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
    * @returns  ProcessRet.
    */
    template <typename ... Args>
    std::tuple<const ProcessRet, const ProcessID> Run(Args&& ... args) {
        PROCESS_INFO("Starting create worker");
        //Create socket pair
        ipc::sock::SockPair pair;
        if (pair.Open() != ret::Return::SUCCESS) {
            PROCESS_ERROR("SockPair Open error.");
            return {ProcessRet::PROCESS_EFIFOPAIR, ProcessID(0)};
        }
        pair.SetAutoClose(false);
        ProcessInfo* parent = ProcessInfo::getInstance();
        pid_t pid = fork();
        if (pid < 0) {
            pair.Close();
            PROCESS_ERROR("Fork error.");
            return {ProcessRet::PROCESS_EFORK, ProcessID(0)};
        } else if (pid == 0) {
            //cache parent process data.
            ProcessParent parent_cache(parent->GetName(), parent->GetPid());
            pair[1].Close();
            parent_cache.SetFD(pair[0]);

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
            child->GetProcessRole().AddRole(ProcessRole::PoolWorker).AddRole(ProcessRole::Child);
            if (config_filename_.empty()) {
                auto config_root = child->GetProcessConfig().GetRoot();
                config_root->Insert("process")->Insert<bool>("switch", false);

                auto config_message = config_root->Insert("message");
                config_message->Insert<bool>("switch", true);
                auto config_message_agent = config_message->Insert("agent");
                config_message_agent->Insert<std::string>("name", "POOL_WORKER");
                config_message_agent->Insert("address")->Insert<std::string>("protocol", "Father-son");
                auto config_message_manager = config_message->Insert("manager");
                config_message_manager->Insert("address")->Insert<std::string>("protocol", "Father-son");
            } else {
                child->GetProcessConfig().LoadFile(config_filename_);
                auto config_root = child->GetProcessConfig().GetRoot();

                if (config_root->Search("process/pool")) {
                    config_root->Search("process/pool")->Erase();
                }
                config_root->Search("process")->Insert("pool")->Insert<bool>("switch", false);

                if (config_root->Search("message")) {
                    config_root->Search("message")->Erase();
                }
                auto config_message = config_root->Insert("message");
                config_message->Insert<bool>("switch", true);
                auto config_message_agent = config_message->Insert("agent");
                config_message_agent->Insert<std::string>("name", "POOL_WORKER");
                config_message_agent->Insert("address")->Insert<std::string>("protocol", "Father-son");
                auto config_message_manager = config_message->Insert("manager");
                config_message_manager->Insert("address")->Insert<std::string>("protocol", "Father-son");
            }
            child->GetProcessConfig().Print();

            message::MessageAgent::getInstance()->Run();

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
            pair[0].Close();
            child.SetFD(pair[1]);
            child.SetDeadCallback(dead_callback_);
            child.GetRole().AddRole(ProcessRole::Child);
            child.SetState(ProcessState::Prepare);

            PROCESS_INFO("Register child [%d] into current process.", pid);
            parent->AddChildProcess(child);
            return {ProcessRet::SUCCESS, child_pid};
        }
        return {ProcessRet::SUCCESS, ProcessID(0)};
    }
private:
    mempool::Mempool* mempool_;         ///< Mempool pointer.
    std::string name_;                  ///< Process Name.
    std::string config_filename_;       ///< Process config file name.
    F child_;                           ///< Process main function.
    void (*dead_callback_)(int*);       ///< Process dead callback for parent SIGCHLD.

    template <typename ... Args>
    ProcessRet _run_main(Args&& ... args) {
        child_(std::forward<Args>(args)...);
        return ProcessRet::SUCCESS;
    }
    
};

};

#endif
