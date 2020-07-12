#ifndef __SOCK_FD_H__
#define __SOCK_FD_H__

#include <netinet/in.h>

#include "timer_time.h"
#include "mempool.h"
#include "io_fd.h"

#include "sock_log.h"
#include "sock_return.h"
#include "sock_address.h"

namespace sock{

/**
* @brief - Socker file descriptor operator class.
*/
class SockFD : public io::FD {
public:
    friend class SockServer;
    friend class SockClient;

    SockFD();
    SockFD(unsigned int fd, bool auto_close = false);
    SockFD(const SockFD& other);
    ~SockFD();

    const SockFD& operator=(const SockFD& other);

    //Inherited from class FD.
    ret::Return SetFD(unsigned int fd, bool auto_close = false);
    ret::Return Dup(io::FD& new_fd);
    io::FD* Clone();
    void Close();
    ssize_t Write(const void* data, size_t datalen);
    ssize_t Read(void* data, size_t datalen);

    //get & set
    SockAddress& GetOrigAddress();
    SockAddress& GetDestAddress();
    SockRet SetOrigAddress(SockAddress& addr);
    SockRet SetDestAddress(SockAddress& addr);

    //for multicast attrubit
    SockRet SetMcastJoin(const char* mcast_addr);
    SockRet SetMcastJoin(const char* mcast_addr, const char* if_name, unsigned int if_index);
    SockRet SetMcastLeave(const char* mcast_addr);
    SockRet SetMcastInterface(const char* if_name, unsigned int if_index);
    SockRet SetMcastTTL(unsigned int ttl);
    SockRet SetMcastloop(bool flag);

    SockRet SetSendBlock(timer::Time* overtime);
    SockRet SetRecvBlock(timer::Time* overtime);
    SockRet SetNonBlock();

    bool isClientFD();
    bool isServerFD();
    bool isAcceptFD();
    bool isMulitcastFD();

    ssize_t Send(SockAddress* dest, const void* data, size_t datalen);
    ssize_t Recv(SockAddress* orig, void* data, size_t datalen);
    ssize_t SendFD(unsigned int fd);
    ssize_t RecvFD(unsigned int *fd);
    
private:
    SockAddress orig;           ///< source address.
    SockAddress dest;           ///< destination address.

    int _close();

    ssize_t _send(struct sockaddr* dest, const void* data, size_t datalen, void* ctrldata, size_t ctrldatalen, int flags);
    ssize_t _recv(struct sockaddr* orig, void* data, size_t datalen, void* ctrldata, size_t ctrldatalen, int flags);
};

}//namespace socket end

#endif
