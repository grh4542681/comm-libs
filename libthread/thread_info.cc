#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "process_info.h"

#include "thread_log.h"
#include "thread_info.h"

#define MAX_THREAD_NAME_LEN (1024)

namespace infra::thread {

thread_local ThreadInfo* ThreadInfo::pInstance = NULL;

ThreadInfo::ThreadInfo()
{
    tid_ = ThreadID((pid_t)syscall(__NR_gettid));
    char name[MAX_THREAD_NAME_LEN];
    memset(name, 0x00, sizeof(name));
    sprintf(name, "_%d", tid_.GetID());
    thread_name_.assign(name);
    thread_ = NULL;
}

ThreadInfo::~ThreadInfo()
{

}

ThreadInfo* ThreadInfo::getInstance()
{
    if (!pInstance) {
        pInstance = mempool::Mempool::getInstance()->Malloc<ThreadInfo>();
    }
    return pInstance;
}

void ThreadInfo::freeInstance()
{
    if (pInstance) {
        mempool::Mempool::getInstance()->Free<ThreadInfo>(pInstance);
        pInstance = NULL;
    }
}

ThreadID& ThreadInfo::GetTid()
{
    return tid_;
}

void ThreadInfo::SetThreadName(const char* name)
{
    if (!name)
        return;
    char cname[MAX_THREAD_NAME_LEN];
    memset(cname, 0x00, sizeof(cname));
    sprintf(cname, "%s_%d", name, tid_.GetID());
    thread_name_.assign(cname);
}

std::string& ThreadInfo::GetThreadName()
{
    return thread_name_;
}

ThreadRet ThreadInfo::Register2Process()
{
    process::ProcessInfo* process_info = process::ProcessInfo::getInstance();
    if (process_info->AddThreadInfo(this) != process::ProcessRet::SUCCESS ) { 
        return ThreadRet::THREAD_EREGISTER;
    }   
    return ThreadRet::SUCCESS; 
}

ThreadRet ThreadInfo::Unregister2Process()
{
    process::ProcessInfo* process_info = process::ProcessInfo::getInstance();
    if (process_info->DelThreadInfo(tid_) != process::ProcessRet::SUCCESS ) { 
        return ThreadRet::THREAD_EUNREGISTER;
    }   
    return ThreadRet::SUCCESS;
}

}
