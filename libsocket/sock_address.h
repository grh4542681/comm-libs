#ifndef __SOCK_ADDRESS_H__
#define __SOCK_ADDRESS_H__

#include <string>

#include <sys/socket.h>
#include "sock_common.h"
#include "sock_return.h"

namespace infra::sock{

/**
* @brief - Socket address operator class.
*/
class SockAddress {
public:
    friend class SockFD;
    friend class SockServer;
    friend class SockClient;
    /**
    * @brief SockAddress - Default constructor
    */
    SockAddress();
    /**
    * @brief SockAddress - Constructor (use default address.)
    *
    * @param [family] - SockFamily.
    * @param [port] - Port number.
    */
    SockAddress(SockFamily family, unsigned short int port):family_(family){
        this->_init(family, NULL, port);
    }
    /**
    * @brief SockAddress - Constructor 
    *
    * @param [family] - SockFamily.
    * @param [address] - Address.
    * @param [port] - Port number.
    */
    SockAddress(SockFamily family, const char* address, unsigned short int port):family_(family){
        this->_init(family, address, port);
    }
    /**
    * @brief SockAddress - Constructor (Local interprocess communication)
    *
    * @param [family] - SpckFamily.
    * @param [address] - File name.
    */
    SockAddress(SockFamily family, const char* address):family_(family){
        this->_init(family, address);
    }

    SockAddress(const SockAddress& other);
    SockAddress(const SockAddress&& other);

    const SockAddress& operator=(const SockAddress& other);
    const SockAddress& operator=(const SockAddress&& other);

    /**
    * @brief getAddr - get address.
    *
    * @returns  address.
    */
    std::string getAddr();

    /**
    * @brief getPort - get port.
    *
    * @returns  port number.
    */

    unsigned short int getPort();
    /**
    * @brief ToSockaddr - Convertor struct sockaddr to SockAddress.
    *
    * @param [addr] - struct sockaddr
    *
    * @returns  SockRet.
    */
    SockRet ToSockaddr(struct sockaddr* addr);
    /**
    * @brief isLocal - Is local SockAddress.
    *
    * @returns  ture/false.
    */
    bool isLocal();
    /**
    * @brief isMulticast - Is multicast SockAddress.
    *
    * @returns  ture/flase.
    */
    bool isMulticast();
    /**
    * @brief AddrCheck - Is this instance available.
    *
    * @returns  ture/false.
    */
    bool AddrCheck();
private:
    bool init_flag_;

    SockFamily family_;
    bool multicast_flag_;
    std::string address_;
    unsigned short int port_;
    int domain_;
    int type_;

    SockRet _init(SockFamily family, unsigned short int port);
    SockRet _init(SockFamily family, const char* address, unsigned short int port);
    SockRet _init(SockFamily family, const char* address);
};

}

#endif
