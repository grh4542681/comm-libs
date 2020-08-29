#ifndef __MSG_H__
#define __MSG_H__

#include <string>

#include "timer_time.h"
#include "ipc_return.h"
#include "ipc_mode.h"

namespace ipc::msg {

class Msg {
public:
    Msg() {
        path_.clear();
    }

    Msg(std::string path) {
        path_ = path;
    }

    virtual ~Msg() { }

    std::string GetPath() {
        return path_;
    }

    virtual IpcRet Create(size_t size, mode_t mode) { return IpcRet::ESUBCLASS; }
    virtual IpcRet Destroy() { return IpcRet::ESUBCLASS; }
    virtual IpcRet Open(IpcMode mode) { return IpcRet::ESUBCLASS; }
    virtual IpcRet Close() { return IpcRet::ESUBCLASS; }

    virtual size_t Recv(void* data, size_t data_len, timer::Time* overtime) { return 0; }
    virtual size_t Send(void* data, size_t data_len, timer::Time* overtime) { return 0; }

protected:
    std::string path_ = {""};
    bool init_flag_ = {false};
};

}

#endif
