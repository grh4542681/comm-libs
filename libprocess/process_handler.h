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
#include "weak_key.h"

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
    ~Handler();

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
    Handler& SetRealName(std::string name);
    Handler& SetCmdLine(int argc, char** argv, char** env);
    Handler& SetCmdLine(char** raw_cmdline, unsigned int raw_cmdline_size);

    Handler& UpdateRealName();
    Handler& UpdateRealPath();

    Return AddParent(Parent& parent);
    Return AddParent(Parent&& parent);
    Return DelParent(std::string name);
    Return DelParent();
    std::tuple<Return, Parent&> GetParent(std::string name);

    Return AddChild(Child& child);
    Return AddChild(Child&& child);
    Return DelChild(std::string name);
    Return DelChild();
    std::tuple<Return, Child&> GetChild(std::string name);

    static Handler& Instance();

    static Return GetProcessRealPath(std::string& path);
    static Return GetProcessRealName(std::string& name);

private:
    Handler();
    Handler(Handler& other);

private:
    // process argement
    ID               pid_;           ///< Process id.
    std::string      name_;          ///< User defined process name.
    std::string      real_path_;     ///< Process exec path.
    std::string      real_name_;     ///< Real process name.
    State            state_;         ///< Process state.
    Role             role_;          ///< Process role.

    // Command line argument
    char**              raw_cmdline_;       ///< Original command line parameter.
    unsigned int        raw_cmdline_size_;  ///< Size of original command line.
    std::vector<char*>  cmdline_;           ///< Command line arguments vector.
    std::vector<char*>  environ_;           ///< Environment arguments vector.

    // process relationship
    std::map<std::string, Parent> parent_map_;    ///< Parent process info.
    std::map<std::string, Child>  child_map_;     ///< Child process info.

    static Handler* pInstance;
};

};

#endif
