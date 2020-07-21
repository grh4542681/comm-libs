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
#include "signal/process_signal_ctrl.h"

namespace process {
class Process;
class ProcessInfo : public report::VReport {
public:
    friend class mempool::Mempool;
    friend class Process;
public:

    ProcessID& GetPid();
    std::string& GetName();
    std::string& GetRealName();
    std::string& GetRealPath();
    ProcessState& GetProcessState();
    ProcessRole& GetProcessRole();
    std::string& GetProcessPoolName();
    const char* GetCmdLine(unsigned int index);
    ProcessInfo& GetCmdLine(char*** raw_cmdline, unsigned int* raw_cmdline_size);
    ProcessConfig& GetProcessConfig();
    parser::Parser& GetProcessRawConfig();
    signal::ProcessSignalCtrl* GetSignalCtrl();

    ProcessInfo& SetPid(ProcessID&& pid);
    ProcessInfo& SetName(std::string name);
    ProcessInfo& SetRealName();
    ProcessInfo& SetRealPath();
    ProcessInfo& SetCmdLine(int argc, char** argv, char** env);
    ProcessInfo& SetCmdLine(char** raw_cmdline, unsigned int raw_cmdline_size);
    template < typename T , typename ... Args> ProcessRet SetProcessRawConfig(Args&& ... args) {
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
            return ProcessRet::EMALLOC;
        }
        return ProcessRet::SUCCESS;
    }

    ProcessRet LoadProcessRawConfig(std::string filename);

    ProcessRet AddThreadInfo(thread::ThreadInfo* thread_info);
    ProcessRet DelThreadInfo(thread::ThreadID& tid);

    ProcessRet AddParentProcess(ProcessParent& parent);
    ProcessRet AddParentProcess(ProcessParent&& parent);
    ProcessRet DelParentProcess();
    ProcessParent* GetParentProcess();

    ProcessRet AddChildProcess(ProcessChild& child);
    ProcessRet AddChildProcess(ProcessChild&& child);
    ProcessRet DelChildProcess(ProcessID& pid);
    ProcessRet DelChildProcess(ProcessID&& pid);
    ProcessRet DelChildProcess(std::string name);
    ProcessChild* GetChildProcess(ProcessID& pid);
    ProcessChild* GetChildProcess(ProcessID&& pid);
    ProcessRet GetChildProcess(std::string name, std::vector<ProcessChild*> child_vector);

    void Report(file::File& fd, report::ReportMode mode);
    void Report(std::stringstream& ss, report::ReportMode mode);

    static ProcessInfo* getInstance();
    static void setInstance(ProcessInfo* info);


private:
    ProcessInfo();
    ProcessInfo(ProcessInfo& other);
    ~ProcessInfo();
public:

private:
    mempool::Mempool* mempool_;

    // base argement
    ProcessID           pid_;           ///< Process id.
    std::string         name_;          ///< User defined process name.
    std::string         real_path_;     ///< Process exec path.
    std::string         real_name_;     ///< Real process name.
    ProcessState        state_;         ///< Process state.
    ProcessRole         role_;          ///< Process role.
    std::string         pool_name_;     ///< Process belong to which process pool.

    // Command line argument
    char**              raw_cmdline_;       ///< Original command line parameter.
    unsigned int        raw_cmdline_size_;  ///< Size of original command line.
    std::vector<char*>  cmdline_;           ///< Command line arguments vector.
    std::vector<char*>  environ_;           ///< Environment arguments vector.

    // process static config
    parser::Parser* raw_config_ = NULL;
    ProcessConfig config_;

    // process relationship
    ProcessParent*                      parent_;    ///< Parent process info.
    std::map<ProcessID, ProcessChild*>  child_;     ///< Child process info.

    // thread info
    thread::mutex::ThreadRWLock thread_info_rw_lock_;       ///< Mutex lock of thread map.
    std::map<thread::ThreadID, thread::ThreadInfo*> thread_info_map_;  ///< Map of all thread in this process.

    // signal
    signal::ProcessSignalCtrl* sig_ctrl_;

    static ProcessInfo* pInstance;
};

};

#endif
