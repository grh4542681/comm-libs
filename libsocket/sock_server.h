#ifndef __SOCK_SERVER_H__
#define __SOCK_SERVER_H__

#include <memory>
#include <string.h>

#include "mempool.h"
#include "sock_return.h"
#include "sock_common.h"
#include "sock_address.h"
#include "sock_fd.h"

namespace infra::sock{

/**
* @brief - Socket server class.
*/
class SockServer {
public:
    SockServer();
    SockServer(SockFamily family, unsigned short int port);
    SockServer(SockFamily family, const char* address, unsigned short int port);
    SockServer(SockFamily family, const char* address);
    SockServer(SockAddress* address);
    SockServer(const SockServer&);
    ~SockServer();

    const SockServer& operator=(const SockServer&);
    /**
    * @brief getSockFD - Get current socket file descriptor.
    *
    * @returns  SockFD pointer.
    */
    SockFD& GetSockFD();

    /**
    * @brief Bind - bind
    *
    * @returns  SockRet.
    */
    SockRet Bind();
    /**
    * @brief Accept - accept.
    *
    * @param [sockfd] - Output arguments SockFd pointer.
    *
    * @returns  SockRet.
    */
    SockRet Accept(SockFD* sockfd);

    SockAddress s_address_;
private:
    bool init_flag_;
    bool auto_close_flag_;
    unsigned int listen_cache_;
    SockFD listen_fd_;

    SockRet _socket();
    SockRet _bind();
    SockRet _listen();
};

}//namespace infra::sock end

#endif
