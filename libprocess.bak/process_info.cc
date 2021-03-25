#include <string.h>

#include "file_api.h"
#include "process.h"
#include "process_info.h"

namespace infra::process {

ProcessInfo* ProcessInfo::pInstance = NULL;

ProcessInfo::ProcessInfo()
{
    mempool_ = mempool::Mempool::getInstance();
    pid_ = ProcessID::GetProcessID();
    if (Process::GetProcRealPath(real_path_) != ProcessRet::SUCCESS) {
        PROCESS_ERROR("Get process path error");
        real_path_.erase();
    }
    if (Process::GetProcRealName(real_name_) != ProcessRet::SUCCESS) {
        PROCESS_ERROR("Get process name error");
        real_name_.erase();
    }
    name_.clear();
    state_ = ProcessState::Normal;
    role_ = ProcessRole::Normal;
    raw_cmdline_ = NULL;
    raw_cmdline_size_ = 0;
    parent_ = NULL;

    sig_ctrl_ = signal::ProcessSignalCtrl::getInstance();
}

ProcessInfo::ProcessInfo(ProcessInfo& other)
{
    mempool_ = other.mempool_;
    pid_ = other.pid_;
    name_ = other.name_;
    state_ = other.state_;
    role_ = other.role_;
    parent_ = NULL;

    raw_cmdline_ = other.raw_cmdline_;
    raw_cmdline_size_ = other.raw_cmdline_size_;

    sig_ctrl_ = other.sig_ctrl_;
}

ProcessInfo::~ProcessInfo()
{

}

ProcessID& ProcessInfo::GetPid()
{
    return pid_;
}

std::string& ProcessInfo::GetName()
{
    return name_;
}

std::string& ProcessInfo::GetRealName()
{
    return real_name_;
}

std::string& ProcessInfo::GetRealPath()
{
    return real_path_;
}

ProcessState& ProcessInfo::GetProcessState()
{
    return state_;
}

ProcessRole& ProcessInfo::GetProcessRole()
{
    return role_;
}

const char* ProcessInfo::GetCmdLine(unsigned int index)
{
    if (index < 0 || index > cmdline_.size())
        return NULL;
    return cmdline_[index];
}

ProcessInfo& ProcessInfo::GetCmdLine(char*** raw_cmdline, unsigned int* raw_cmdline_size)
{
    *raw_cmdline = raw_cmdline_;
    *raw_cmdline_size = raw_cmdline_size_;
    return *this;
}

parser::Parser& ProcessInfo::GetProcessRawConfig()
{
    return *raw_config_;
}

ProcessConfig& ProcessInfo::GetProcessConfig()
{
    return config_;
}

signal::ProcessSignalCtrl* ProcessInfo::GetSignalCtrl()
{
    return sig_ctrl_;
}

ProcessInfo& ProcessInfo::SetPid(ProcessID&& pid)
{
    pid_ = pid;
    return *this;
}

ProcessInfo& ProcessInfo::SetName(std::string name)
{
    name_ = name;
    return *this;
}

ProcessInfo& ProcessInfo::SetRealName()
{
    if (Process::GetProcRealName(real_name_) != ProcessRet::SUCCESS) {
        PROCESS_ERROR("Get process name error");
        real_name_.erase();
    }
    return *this;
}

ProcessInfo& ProcessInfo::SetRealPath()
{
    if (Process::GetProcRealPath(real_path_) != ProcessRet::SUCCESS) {
        PROCESS_ERROR("Get process path error");
        real_path_.erase();
    }
    return *this;
}

ProcessInfo& ProcessInfo::SetCmdLine(int argc, char** argv, char** env)
{
    int loop = 0;
    char* tmp = NULL;

    // back up command line argv
    if (argv) {
        for (loop = 0; loop < argc; loop++) {
            if (strlen(argv[loop]) >= 0) {
                tmp = (char*)(mempool_->Malloc(strlen(argv[loop]) + 1));
                memset(tmp, 0, strlen(argv[loop]));
                memcpy(tmp, argv[loop], strlen(argv[loop]));
                cmdline_.push_back(tmp);
                raw_cmdline_size_ += strlen(argv[loop]) + 1;
            }
        }
        raw_cmdline_ = argv;
    }

    // back up environ args
    if (env) {
        for (loop = 0; env[loop] != NULL; loop++) {
            if (strlen(env[loop]) >= 0) {
                tmp = (char*)(mempool_->Malloc(strlen(env[loop]) + 1));
                memset(tmp, 0, strlen(env[loop]));
                memcpy(tmp, env[loop], strlen(env[loop]));
                cmdline_.push_back(tmp);
                raw_cmdline_size_ += strlen(env[loop]) + 1;
            }
        }
    }
    return *this;
}

ProcessInfo& ProcessInfo::SetCmdLine(char** raw_cmdline, unsigned int raw_cmdline_size)
{
    raw_cmdline_ = raw_cmdline;
    raw_cmdline_size_ = raw_cmdline_size;
    return *this;
}

ProcessRet ProcessInfo::LoadProcessRawConfig(std::string filename)
{
    file::File config_file(filename);
    switch(config_file.GetFileFormat()) {
//        case file::File::Format::Ini:
        case file::File::Format::Json:
            if (SetProcessRawConfig<parser::ParserJson>() != ProcessRet::SUCCESS) {
                PROCESS_ERROR("Con't set process raw config with type Json");
                return ProcessRet::PROCESS_ERAWCONFIG;
            }
            raw_config_->LoadFile(config_file);
            break;
        case file::File::Format::Yaml:
            if (SetProcessRawConfig<parser::ParserYaml>() != ProcessRet::SUCCESS) {
                PROCESS_ERROR("Con't set process raw config with type Yaml");
                return ProcessRet::PROCESS_ERAWCONFIG;
            }
            raw_config_->LoadFile(config_file);
            break;
        default:
            PROCESS_ERROR("Not support config file format [%s]", file::GetFileExtension(filename).c_str());
            return ProcessRet::PROCESS_ERAWCONFIG;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessInfo::AddParentProcess(ProcessParent& parent)
{
    return AddParentProcess(std::move(parent));
}
ProcessRet ProcessInfo::AddParentProcess(ProcessParent&& parent)
{
    if (parent_) {
        mempool_->Reset<ProcessParent>(parent_, parent);
    } else {
        parent_ = mempool_->Malloc<ProcessParent>(parent);
    }
    return parent_ ? ProcessRet::SUCCESS : ProcessRet::EMALLOC;
}

ProcessRet ProcessInfo::DelParentProcess()
{
    if (parent_) {
        mempool_->Free<ProcessParent>(parent_);
    }
    return ProcessRet::SUCCESS;
}

ProcessParent* ProcessInfo::GetParentProcess()
{
    return parent_;
}

ProcessRet ProcessInfo::AddChildProcess(ProcessChild& child)
{
    return AddChildProcess(std::move(child));
}
ProcessRet ProcessInfo::AddChildProcess(ProcessChild&& child)
{
    auto it = child_.find(child.GetPid());
    if (it != child_.end()) {
        return ProcessRet::PROCESS_EPROCDUP;
    }
    child.GetFD().SetAutoClose(false);
    ProcessChild* p = mempool_->Malloc<ProcessChild>(child);
    if (!p) {
        return ProcessRet::PROCESS_EMEMORY;
    }
    std::pair<std::map<ProcessID, ProcessChild*>::iterator, bool> ret;
    ret = child_.insert(std::pair<ProcessID, ProcessChild*>(child.GetPid(), p));
    if (ret.second == false) {
        return ProcessRet::PROCESS_EPROCADD;
    }
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessInfo::DelChildProcess(ProcessID& pid)
{
    return DelChildProcess(std::move(pid));
}
ProcessRet ProcessInfo::DelChildProcess(ProcessID&& pid)
{
    auto it = child_.find(pid);
    if (it == child_.end()) {
        return ProcessRet::PROCESS_EPROCNOTFOUND;
    }
    mempool_->Free<ProcessChild>(it->second);
    child_.erase(it);
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessInfo::DelChildProcess(std::string name)
{
    for (auto& it : child_) {
        if (!(it.second->GetName().compare(name))) {
            mempool_->Free<ProcessChild>(it.second);
            child_.erase(it.first);
        }
    }
    return ProcessRet::SUCCESS;
}

ProcessChild* ProcessInfo::GetChildProcess(ProcessID& pid)
{
    return GetChildProcess(std::move(pid));
}
ProcessChild* ProcessInfo::GetChildProcess(ProcessID&& pid)
{
    auto it = child_.find(pid);
    if (it == child_.end()) {
        return NULL;
    }
    return it->second;
}

ProcessRet ProcessInfo::GetChildProcess(std::string name, std::vector<ProcessChild*> child_vector)
{
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessInfo::AddThreadInfo(thread::ThreadInfo* thread_info)
{
    thread_info_rw_lock_.WLock(NULL);
    auto it = thread_info_map_.find(thread_info->GetTid());
    if (it != thread_info_map_.end()) {
        thread_info_rw_lock_.UnLock();
        return ProcessRet::PROCESS_ETHREADDUP;
    }
    std::pair<std::map<thread::ThreadID, thread::ThreadInfo*>::iterator, bool> ret;
    ret = thread_info_map_.insert(std::pair<thread::ThreadID, thread::ThreadInfo*>(thread_info->GetTid(), thread_info));
    if (ret.second==false) {
        thread_info_rw_lock_.UnLock();
        return ProcessRet::PROCESS_ETHREADADD;
    }
    thread_info_rw_lock_.UnLock();
    return ProcessRet::SUCCESS;
}

ProcessRet ProcessInfo::DelThreadInfo(thread::ThreadID& tid)
{
    thread_info_rw_lock_.WLock(NULL);
    thread_info_map_.erase(tid);
    thread_info_rw_lock_.UnLock();
    return ProcessRet::SUCCESS;
}

void ProcessInfo::Report(file::File& fd, report::ReportMode mode)
{
    thread_info_rw_lock_.RLock(NULL);
    fd.GetFileFD().WriteFmt("pid:%u threadnum:%d processnum:%d\n", pid_.GetID(), thread_info_map_.size(), child_.size());
    for (auto it : thread_info_map_) {
        fd.GetFileFD().WriteFmt("\ttid:%d\n", it.first.GetID());
    }
    thread_info_rw_lock_.UnLock();
}

void ProcessInfo::Report(std::stringstream& ss, report::ReportMode mode)
{

}

//static
ProcessInfo* ProcessInfo::getInstance()
{
    if (!pInstance) {
        mempool::Mempool* mp = mempool::Mempool::getInstance();
        pInstance = mp->Malloc<ProcessInfo>();
    }
    return pInstance;
}

void ProcessInfo::setInstance(ProcessInfo* info)
{
    pInstance = info;
}

}
