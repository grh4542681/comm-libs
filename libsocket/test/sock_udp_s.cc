#include "sock_common.h"
#include "sock_address.h"
#include "sock_fd.h"
#include "sock_server.h"
#include <unistd.h>

int main()
{
    char buff[1024];
    std::string unixfile = "sock.sock";
    //sock::SockServer s = sock::SockServer(sock::SockFamily::UDP_LOCAL, unixfile.c_str());
    //sock::SockServer s = sock::SockServer(sock::SockFamily::UDP_INET4, 60005);
    //sock::SockServer s = sock::SockServer(sock::SockFamily::UDP_INET6, 60005);
    sock::SockServer s = sock::SockServer(sock::SockFamily::MULTICAST_INET4, NULL, 60005);
    s.Bind();
    sock::SockFD fd = s.GetSockFD();
    fd.SetMcastJoin("228.0.0.1");
    while(1){
        sleep(1);
        sock::SockAddress addr;
        memset(buff, 0x00, sizeof(buff));
        int ret = fd.Recv(&addr, buff, sizeof(buff));
        printf("ret[%d][%s][%s]\n", ret, buff, addr.getAddr().c_str());
    }
    return 0;
}
