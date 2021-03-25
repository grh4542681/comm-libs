#ifndef __PROCESS_GROUP_WORKER_TEMPLATE_H__
#define __PROCESS_GROUP_WORKER_TEMPLATE_H__

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

namespace infra::process::group {

template < typename F, typename ... Args >
class ProcessGroupWorkerTemplate {
public:
    /**
    * @brief ProcessGroupWorkerTemplate - Constructor
    *
    * @param [child] - process main function.
    */
    ProcessGroupWorkerTemplate(std::string name, F child, Args&& ... args) : tuple_args(std::forward_as_tuple(std::forward<Args>(args)...)) {
        mempool_ = mempool::Mempool::getInstance();
        name_ = name;
        config_filename_.clear();
        child_ = child;
    }

    /**
    * @brief ProcessGroupWorkerTemplate - Constructor
    *
    * @param [name] - Process name.
    * @param [child] - Process main function.
    */
    ProcessGroupWorkerTemplate(std::string name, std::string config_filename, F child, Args&& ... args) : tuple_args(std::forward_as_tuple(std::forward<Args>(args)...)) {
        mempool_ = mempool::Mempool::getInstance();
        name_ = name;
        config_filename_ = config_filename;
        child_ = child;
    }

    ~ProcessGroupWorkerTemplate() {

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
    *
    * @returns  ProcessRet.
    */
    std::tuple<const ProcessRet, const ProcessID> Run() {
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
            //pair[1].Close();
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
                config_root->Insert("process")->Insert("group")->Insert<bool>("switch", false);

                auto config_message = message::MessageAgent::getInstance()->GetConfig().GetRoot()->Insert("message");
                config_message->Insert<bool>("switch", true);
                auto config_message_agent = config_message->Insert("agent");
                config_message_agent->Insert<std::string>("name", "GROUP_WORKER");
                config_message_agent->Insert("address")->Insert<std::string>("protocol", "Keeper-Worker");
                auto config_message_manager = config_message->Insert("manager");
                config_message_manager->Insert<bool>("switch", true);
                config_message_manager->Insert("address")->Insert<std::string>("protocol", "Keeper-Worker");
            } else {
                child->LoadProcessRawConfig(config_filename_);
                child->GetProcessConfig().Load(child->GetProcessRawConfig());
                auto config_root = child->GetProcessConfig().GetRoot();

                if (config_root->Search("process/group")) {
                    config_root->Search("process/group")->Erase();
                }
                config_root->Search("process")->Insert("group")->Insert<bool>("switch", false);

                auto config_message = message::MessageAgent::getInstance()->GetConfig().GetRoot()->Insert("message");
                config_message->Insert<bool>("switch", true);
                auto config_message_agent = config_message->Insert("agent");
                config_message_agent->Insert<std::string>("name", "GROUP_WORKER");
                config_message_agent->Insert("address")->Insert<std::string>("protocol", "Keeper-Worker");
                auto config_message_manager = config_message->Insert("manager");
                config_message_manager->Insert<bool>("switch", true);
                config_message_manager->Insert("address")->Insert<std::string>("protocol", "Keeper-Worker");
            }
    //        child->GetProcessConfig().Print();

            message::MessageAgent::getInstance()->RegisterManager(child->GetParentProcess()->GetFD(), MessageManagerCallback);
            message::MessageAgent::getInstance()->Run();

            PROCESS_INFO("Execute child main function.");
            _run_main(std::make_index_sequence<std::tuple_size<std::tuple<Args...>>::value>());
            exit(0);
        } else {
            std::string child_name = name_;
            if (child_name.empty()) {
                child_name = parent->GetName() + "_" + std::to_string(pid);
            }
            ProcessID child_pid(pid);
            ProcessChild child(child_name, std::move(child_pid));
            //pair[0].Close();
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

    static io::IoRet MessageManagerCallback(message::MessageManager* manager, io::SelectItemTemplate<message::MessageManager>* item, int events) {
        printf("%s : %d ---callback--\n", __FILE__, __LINE__);
        auto fd = item->template GetFd<sock::SockFD>();
        char buff[1024];
        memset(buff, 0, sizeof(buff));
        fd.Recv(NULL,buff,sizeof(buff));
        printf("recv %s\n", buff);
        return ProcessRet::SUCCESS;
    }

private:
    mempool::Mempool* mempool_;         ///< Mempool pointer.
    std::string name_;                  ///< Process Name.
    std::string config_filename_;       ///< Process config file name.
    F child_;                           ///< Process main function.
    std::tuple<Args...> tuple_args;     ///< process main function args.
    void (*dead_callback_)(int*);       ///< Process dead callback for parent SIGCHLD.

    template <std::size_t... I> ProcessRet _run_main(std::index_sequence<I...>) {
       child_(std::get<I>(tuple_args)...);
       return ProcessRet::SUCCESS;
    }
    
};

};

#endif
