#ifndef __PROCESS_INFO_H__
#define __PROCESS_INFO_H__

#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <map>
#include <vector>
#include <memory>

#include "object.h"
#include "allocator.h"

#include "process_log.h"
#include "process_return.h"
#include "process_id.h"
#include "process_role.h"
#include "process_state.h"
#include "process_parent.h"
#include "process_child.h"

#define  MAX_PROCCESS_NAME_LEN (1024)
#define  MAX_PROCCESS_PATH_LEN (1024)

namespace infra::process {

class Handler : virtual public base::Object {
public:
    ID& GetPid();
    std::string& GetName();
    std::string& GetRealName();
    std::string& GetRealPath();
    State& GetState();
    Role& GetRole();
    const char* GetCmdLine(unsigned int index);
    Handler& GetCmdLine(char*** raw_cmdline, unsigned int* raw_cmdline_size);

    Handler& SetPid(ID&& pid);
    Handler& SetName(std::string name);
    Handler& UpdateRealName();
    Handler& UpdateRealPath();
    Handler& SetCmdLine(int argc, char** argv, char** env);
    Handler& SetCmdLine(char** raw_cmdline, unsigned int raw_cmdline_size);

    template <typename ... Args> Return AddParent(Args ... args) {
        if (parent_) {
            parent_ = alloc_.Reconstruct<Parent>(parent_, std::forward<Args>(args)...);
        } else {
            parent_ = alloc_.Allocate<Parent>(std::forward<Args>(args)...);
        }
        return parent_ ? Return::SUCCESS : Return::PROCESS_EMEMORY;
    }
    Return AddParent(Parent& parent) {
        return AddParent(std::move(parent));
    }
    Return AddParent(Parent&& parent) {
        parent.SetAutoClose(false);
        if (parent_) {
            parent_ = alloc_.Reconstruct<Parent>(parent_, parent);
        } else {
            parent_ = alloc_.Allocate<Parent>(parent);
        }
        return parent_ ? Return::SUCCESS : Return::PROCESS_EMEMORY;
    }
    Return DelParent();
    Parent* GetParent();

    Return AddChild(Child& child);
    Return AddChild(Child&& child);
    Return DelChild(ID& pid);
    Return DelChild(ID&& pid);
    Return DelChild(std::string name);
    Child* GetChild(ID& pid);
    Child* GetChild(ID&& pid);
    Return GetChild(std::string name, std::vector<Child*> child_vector);

    static Handler* GetInstance(base::Allocator&& alloc = base::Allocator());
    static void SetInstance(Handler* info);

    static Return GetProcessRealPath(std::string& path);
    static Return GetProcessRealName(std::string& name);

private:
    Handler(base::Allocator&& alloc = base::Allocator());
    Handler(Handler& other);
    ~Handler();

private:
    // process argement
    ID               pid_;           ///< Process id.
    std::string      name_;          ///< User defined process name.
    std::string      real_path_;     ///< Process exec path.
    std::string      real_name_;     ///< Real process name.
    State            state_;         ///< Process state.
    Role             role_;          ///< Process role.
    base::Allocator  alloc_;

    // Command line argument
    char**              raw_cmdline_;       ///< Original command line parameter.
    unsigned int        raw_cmdline_size_;  ///< Size of original command line.
    std::vector<char*>  cmdline_;           ///< Command line arguments vector.
    std::vector<char*>  environ_;           ///< Environment arguments vector.

    // process relationship
    Parent*               parent_;    ///< Parent process info.
    std::map<ID, Child*>  child_map_;     ///< Child process info.

    static Handler* pInstance;
};

};

#endif
