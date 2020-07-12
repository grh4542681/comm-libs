#ifndef __SOCK_COMMON_H__
#define __SOCK_COMMON_H__

namespace sock {

#define SOCK_ADDRESS_MAX_LEN (256)

/**
* @brief - Socket family.
*/
enum class SockFamily{
    TCP_LOCAL = 0,      ///< TCP Local interprocess communication.
    TCP_INET4,          ///< TCP ipv4.
    TCP_INET6,          ///< TCP ipv6.

    UDP_LOCAL,          ///< UDP Local interprocess communication.
    UDP_INET4,          ///< UDP ipv4.
    UDP_INET6,          ///< UDP ipv6.
    
    MULTICAST_INET4,    ///< Multicast ipv4.
    MULTICAST_INET6,    ///< Multicast ipv6.
};

}//namespace sock end

#endif
