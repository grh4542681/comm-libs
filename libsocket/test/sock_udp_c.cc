#include "sock_common.h"
#include "sock_address.h"
#include "sock_fd.h"
#include "sock_client.h"
#include <unistd.h>

int main()
{
    std::string unixfile = "sock.sock";
    //sock::SockClient s = sock::SockClient(sock::SockFamily::UDP_LOCAL, unixfile.c_str());
    //sock::SockClient s = sock::SockClient(sock::SockFamily::UDP_INET4, NULL, 60005);
    //sock::SockClient s = sock::SockClient(sock::SockFamily::UDP_INET6, NULL, 60005);
    sock::SockClient s = sock::SockClient(sock::SockFamily::MULTICAST_INET4, "228.0.0.1", 60005);
    s.Connect();
    sock::SockFD& fd = s.GetSockFD();
    fd.SetMcastJoin("228.0.0.1");
//    sock::SockAddress address = sock::SockAddress(sock::SockFamily::UDP_INET6, "::", 60005);
//    fd->Send(&address,"hello world", 12);
    fd.Write("hello world", 12);
    fd.Close();
    return 0;
}
