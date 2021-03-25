#ifndef __SOCK_CLIENT_H__
#define __SOCK_CLIENT_H__

#include "mempool.h"
#include "sock_return.h"
#include "sock_common.h"
#include "sock_address.h"
#include "sock_fd.h"

namespace infra::sock{

/**
* @brief - Socket client class.
*/
class SockClient {
public:
    SockClient();
    SockClient(SockFamily family, const char* address, unsigned short int port);
    SockClient(SockFamily family, const char* address);
    SockClient(SockAddress* address);
    SockClient(const SockClient&);
    ~SockClient();

    const SockClient& operator=(const SockClient&);
    /**
    * @brief getSockFD - Get current socket file descriptor.
    *
    * @returns  SockFD pointer.
    */
    SockFD& GetSockFD();
    /**
    * @brief SetTimeout - Set connect over time.
    *
    * @param [overtime] - Over time.
    *
    * @returns  SockRet.
    */
    SockRet SetTimeout(struct timeval* overtime);
    void SetAutoClose(bool flag);
    /**
    * @brief Connect - connect.
    *
    * @returns  SockFD pointer.
    */
    SockRet Connect();

private:

    bool init_flag_;
    bool auto_close_flag_;
    SockAddress s_address_;
    SockFD conn_fd_;

    SockRet _socket();
    SockRet _connect();

};

}// namespace infra::sock end


#endif
