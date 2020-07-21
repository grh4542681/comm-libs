#ifndef __PROCESS_INFO_H__
#define __PROCESS_INFO_H__

#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <map>
#include <vector>

#include "file.h"
#include "report.h"
#include "mempool.h"
#include "thread_id.h"
#include "thread_info.h"
#include "mutex/thread_rw_lock.h"
#include "socket/sock_pair.h"

#include "process_log.h"
#include "process_return.h"
#include "process_config.h"
#include "process_state.h"
#include "process_role.h"
#include "process_id.h"
#include "process_parent.h"
#include "process_child.h"
//#include "signal/process_signal_ctrl.h"

namespace infra::process {
class Handle {
public:
    friend class mempool::Mempool;
    friend class Process;
public:

    ID& GetPid();
    std::string& GetName();
    std::string& GetRealName();
    std::string& GetRealPath();
    State& GetState();
    Role& GetRole();
    std::string& GetProcessPoolName();
    const char* GetCmdLine(unsigned int index);
    Handle& GetCmdLine(char*** raw_cmdline, unsigned int* raw_cmdline_size);
    Config& GetConfig();
    parser::Parser& GetProcessRawConfig();
    signal::SignalCtrl* GetSignalCtrl();

    Handle& SetPid(ID&& pid);
    Handle& SetName(std::string name);
    Handle& SetRealName();
    Handle& SetRealPath();
    Handle& SetCmdLine(int argc, char** argv, char** env);
    Handle& SetCmdLine(char** raw_cmdline, unsigned int raw_cmdline_size);
    template < typename T , typename ... Args> Return SetProcessRawConfig(Args&& ... args) {
        if (raw_config_) {
            std::string type_name = raw_config_->ObjectTypeName();
            if (type_name == "ParserJson") {
                mempool_->Free<parser::ParserJson>(dynamic_cast<parser::ParserJson*>(raw_config_));
            } else if (type_name == "ParserYaml") {
                mempool_->Free<parser::ParserYaml>(dynamic_cast<parser::ParserYaml*>(raw_config_));
            } else {
                // unknow
                mempool_->Free((void*)raw_config_);
            }
            raw_config_ = NULL;
        }
        raw_config_ = mempool_->Malloc<T>(std::forward<Args>(args)...);
        if (!raw_config_) {
            return Return::EMALLOC;
        }
        return Return::SUCCESS;
    }

    Return LoadProcessRawConfig(std::string filename);

    Return AddThreadInfo(thread::ThreadInfo* thread_info);
    Return DelThreadInfo(thread::ThreadID& tid);

    Return AddParentProcess(Parent& parent);
    Return AddParentProcess(Parent&& parent);
    Return DelParentProcess();
    Parent* GetParentProcess();

    Return AddChildProcess(Child& child);
    Return AddChildProcess(Child&& child);
    Return DelChildProcess(ID& pid);
    Return DelChildProcess(ID&& pid);
    Return DelChildProcess(std::string name);
    Child* GetChildProcess(ID& pid);
    Child* GetChildProcess(ID&& pid);
    Return GetChildProcess(std::string name, std::vector<Child*> child_vector);

    void Report(file::File& fd, report::ReportMode mode);
    void Report(std::stringstream& ss, report::ReportMode mode);

    static Handle* getInstance();
    static void setInstance(Handle* info);


private:
    Handle();
    Handle(Handle& other);
    ~Handle();
public:

private:
    mempool::Mempool* mempool_;

    // base argement
    ID           pid_;           ///< Process id.
    std::string         name_;          ///< User defined process name.
    std::string         real_path_;     ///< Process exec path.
    std::string         real_name_;     ///< Real process name.
    State        state_;         ///< Process state.
    Role         role_;          ///< Process role.
    std::string         pool_name_;     ///< Process belong to which process pool.

    // Command line argument
    char**              raw_cmdline_;       ///< Original command line parameter.
    unsigned int        raw_cmdline_size_;  ///< Size of original command line.
    std::vector<char*>  cmdline_;           ///< Command line arguments vector.
    std::vector<char*>  environ_;           ///< Environment arguments vector.

    // process static config
    parser::Parser* raw_config_ = NULL;
    Config config_;

    // process relationship
    Parent*                      parent_;    ///< Parent process info.
    std::map<ID, Child*>  child_;     ///< Child process info.

    // thread info
//    thread::mutex::ThreadRWLock thread_info_rw_lock_;       ///< Mutex lock of thread map.
//    std::map<thread::ThreadID, thread::ThreadInfo*> thread_info_map_;  ///< Map of all thread in this process.

    // signal
    SignalCtrl* sig_ctrl_;

    static Handle* pInstance;
};

};

#endif
