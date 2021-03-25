#include "process_log.h"
#include "process.h"

#include "file.h"

namespace infra::process {


ProcessRet Process::SetProcName(std::string name)
{
    ProcessInfo* info = ProcessInfo::getInstance();
    if (!info || !info->raw_cmdline_) {
        return ProcessRet::EBADARGS;
    }
    if (name.size() >= info->raw_cmdline_size_) {
        name.resize(info->raw_cmdline_size_);
    }
    memcpy(info->raw_cmdline_[0], name.c_str(), name.size());
    memset(info->raw_cmdline_[0] + name.size(), 0, strlen(info->raw_cmdline_[0]) - name.size());
    return ProcessRet::SUCCESS;
}

ProcessRet Process::GetProcRealPath(std::string& path)
{
    char process_path[MAX_PROCCESS_NAME_LEN];
    memset(process_path, 0x00, sizeof(process_path));
    if (readlink("/proc/self/exe", process_path, sizeof(process_path)-1) < 0) {
        int tmperrno = errno;
        PROCESS_ERROR("Get process path error[%s]", strerror(tmperrno));
        return ProcessRet::ERROR;
    }   
    path.assign(process_path);
    return ProcessRet::SUCCESS;
}

ProcessRet Process::GetProcRealName(std::string& name)
{
    char process_name[MAX_PROCCESS_NAME_LEN];
    memset(process_name, 0x00, sizeof(process_name));
    file::File cmdline("/proc/self/cmdline");
    if (cmdline.Open(file::File::Mode::READ_ONLY) != file::FileRet::SUCCESS) {
        return ProcessRet::ERROR;
    }
    cmdline.GetFileFD().Read(process_name, sizeof(process_name));
    cmdline.Close();
    name.assign(process_name);
    return ProcessRet::SUCCESS;
}

ProcessRet Process::SendToParent(char* msg, unsigned int* msglen, timer::Time* overtime)
{
    ProcessInfo* self_info = ProcessInfo::getInstance();
    return ProcessRet::SUCCESS;
}


ProcessRet Process::RecvFromParent(char* msg, unsigned int* msglen, timer::Time* overtime)
{
    ProcessInfo* self_info = ProcessInfo::getInstance();
    return ProcessRet::SUCCESS;
}

}
