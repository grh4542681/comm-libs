#include <string.h>

#include "file.h"
#include "process_handler.h"

namespace infra::process {

Handler* Handler::pInstance = NULL;

Handler::Handler(base::Allocator&& alloc) : alloc_(alloc)
{
    pid_ = ID::GetProcessID();
    if (GetProcessRealPath(real_path_) != Return::SUCCESS) {
        Log::Error("Get process path error");
        real_path_.erase();
    }
    if (GetProcessRealName(real_name_) != Return::SUCCESS) {
        Log::Error("Get process name error");
        real_name_.erase();
    }
    name_ = real_name_;
    state_ = State::Normal;
    role_ = Role::Normal;

    raw_cmdline_ = NULL;
    raw_cmdline_size_ = 0;
}

Handler::~Handler()
{

}

ID& Handler::GetPid()
{
    return pid_;
}

std::string& Handler::GetName()
{
    return name_;
}

std::string& Handler::GetRealName()
{
    return real_name_;
}

std::string& Handler::GetRealPath()
{
    return real_path_;
}

State& Handler::GetState()
{
    return state_;
}

Role& Handler::GetRole()
{
    return role_;
}

const char* Handler::GetCmdLine(unsigned int index)
{
    if (index < 0 || index > cmdline_.size())
        return NULL;
    return cmdline_[index];
}

Handler& Handler::GetCmdLine(char*** raw_cmdline, unsigned int* raw_cmdline_size)
{
    *raw_cmdline = raw_cmdline_;
    *raw_cmdline_size = raw_cmdline_size_;
    return *this;
}

Handler& Handler::SetPid(ID&& pid)
{
    pid_ = pid;
    return *this;
}

Handler& Handler::SetName(std::string name)
{
    name_ = name;
    return *this;
}

Handler& Handler::UpdateRealName()
{
    if (GetProcessRealName(real_name_) != Return::SUCCESS) {
        Log::Error("Get process name error");
        real_name_.erase();
    }
    return *this;
}

Handler& Handler::UpdateRealPath()
{
    if (GetProcessRealPath(real_path_) != Return::SUCCESS) {
        Log::Error("Get process path error");
        real_path_.erase();
    }
    return *this;
}

Handler& Handler::SetCmdLine(int argc, char** argv, char** env)
{
    int loop = 0;
    char* tmp = NULL;

    // back up command line argv
    if (argv) {
        for (loop = 0; loop < argc; loop++) {
            if (strlen(argv[loop]) >= 0) {
                tmp = (char*)(alloc_.Malloc(strlen(argv[loop]) + 1));
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
                tmp = (char*)(alloc_.Malloc(strlen(env[loop]) + 1));
                memset(tmp, 0, strlen(env[loop]));
                memcpy(tmp, env[loop], strlen(env[loop]));
                cmdline_.push_back(tmp);
                raw_cmdline_size_ += strlen(env[loop]) + 1;
            }
        }
    }
    return *this;
}

Handler& Handler::SetCmdLine(char** raw_cmdline, unsigned int raw_cmdline_size)
{
    raw_cmdline_ = raw_cmdline;
    raw_cmdline_size_ = raw_cmdline_size;
    return *this;
}

Return Handler::AddParent(Parent& parent)
{
    return AddParent(std::move(parent));
}
Return Handler::AddParent(Parent&& parent)
{
    if (parent_map_.find(parent.GetPid()) != parent_map_.end()) {
        return Return::PROCESS_PARENT_EEXIST;
    }
    parent.SetAutoClose(false);
    auto [it, success] = parent_map_.insert({ID(parent.GetPid()), Parent(std::forward<Parent>(parent))});
    if (!success) {
        return Return::PROCESS_PARENT_EINSERT;
    }
    it->second.SetAutoClose(true);
    return Return::SUCCESS;
}

Return Handler::DelParent(ID& pid)
{
    return DelParent(std::move(pid));
}
Return Handler::DelParent(ID&& pid)
{
    if (parent_map_.find(pid) == parent_map_.end()) {
        return Return::PROCESS_PARENT_ENOTEXIST;
    }
    parent_map_.erase(pid);
    return Return::SUCCESS;
}

std::tuple<Return, Parent&> Handler::GetParent(ID& pid)
{
    return GetParent(std::move(pid));
}
std::tuple<Return, Parent&> Handler::GetParent(ID&& pid)
{
    auto it = parent_map_.find(pid);
    if (it == parent_map_.end()) {
        return {Return::PROCESS_PARENT_ENOTEXIST, it->second};
    }
    return {Return::SUCCESS, it->second};
}

//Return Handler::AddChild(Child& child)
//{
//    return AddChild(std::move(child));
//}
//Return Handler::AddChild(Child&& child)
//{
//    auto it = child_map_.find(child.GetPid());
//    if (it != child_map_.end()) {
//        return Return::PROCESS_EPROCDUP;
//    }
//    child.SetAutoClose(false);
//    Child* p = alloc_.Allocate<Child>(child);
//    if (!p) {
//        return Return::PROCESS_EMEMORY;
//    }
//    std::pair<std::map<ID, Child*>::iterator, bool> ret;
//    ret = child_map_.insert(std::pair<ID, Child*>(child.GetPid(), p));
//    if (ret.second == false) {
//        return Return::PROCESS_EPROCADD;
//    }
//    return Return::SUCCESS;
//}
//
//Return Handler::DelChild(ID& pid)
//{
//    return DelChild(std::move(pid));
//}
//Return Handler::DelChild(ID&& pid)
//{
//    auto it = child_map_.find(pid);
//    if (it == child_map_.end()) {
//        return Return::PROCESS_EPROCNOTFOUND;
//    }
//    alloc_.Deallocate<Child>(it->second);
//    child_map_.erase(it);
//    return Return::SUCCESS;
//}
//
//Return Handler::DelChild(std::string name)
//{
//    for (auto& it : child_map_) {
//        if (!(it.second->GetName().compare(name))) {
//            alloc_.Deallocate<Child>(it.second);
//            child_map_.erase(it.first);
//        }
//    }
//    return Return::SUCCESS;
//}
//
//Child* Handler::GetChild(ID& pid)
//{
//    return GetChild(std::move(pid));
//}
//Child* Handler::GetChild(ID&& pid)
//{
//    auto it = child_map_.find(pid);
//    if (it == child_map_.end()) {
//        return NULL;
//    }
//    return it->second;
//}
//
//Return Handler::GetChild(std::string name, std::vector<Child*> child_vector)
//{
//    return Return::SUCCESS;
//}

//static
Handler* Handler::GetInstance(base::Allocator&& alloc)
{
    if (!pInstance) {
        pInstance = new Handler(std::forward<base::Allocator>(alloc));
    }
    return pInstance;
}

void Handler::SetInstance(Handler* info)
{
    pInstance = info;
}

Return Handler::GetProcessRealPath(std::string& path)
{
    char process_path[MAX_PROCCESS_PATH_LEN];
    memset(process_path, 0x00, sizeof(process_path));
    if (readlink("/proc/self/exe", process_path, sizeof(process_path)-1) < 0) {
        int tmperrno = errno;
        Log::Error("Get process path error[%s]", strerror(tmperrno));
        return Return::ERROR;
    }   
    path.assign(process_path);
    return Return::SUCCESS;
}

Return Handler::GetProcessRealName(std::string& name)
{
    char process_name[MAX_PROCCESS_NAME_LEN];
    memset(process_name, 0x00, sizeof(process_name));
    file::File cmdline("/proc/self/cmdline");
    if (cmdline.Open(file::File::Mode::READ_ONLY) != file::Return::SUCCESS) {
        return Return::ERROR;
    }
    cmdline.GetFileFD().Read(process_name, sizeof(process_name));
    cmdline.Close();
    name.assign(process_name);
    return Return::SUCCESS;
}

}
