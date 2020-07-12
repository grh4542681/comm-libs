#ifndef __SOCK_PAIR_H__
#define __SOCK_PAIR_H__

#include "ipc_log.h"
#include "ipc_return.h"

#include "sock_fd.h"
namespace socklib = sock;
namespace ipc::sock {

class SockPair {
public:
    SockPair();
    SockPair(SockPair& other);
    ~SockPair();

    void SetAutoClose(bool flag);

    IpcRet Open();
    void Close();

    socklib::SockFD& operator[] (const unsigned int index);
    SockPair& operator= (SockPair& other);

private:
    bool init_flag_;
    bool auto_close_;
    socklib::SockFD fds_[3];
    
};

}

#endif
