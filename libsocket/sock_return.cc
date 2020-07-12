#include "sock_return.h"

namespace sock {

SockRet::ECodeMapType SockRet::ECodeMap = {
    { ECode::SOCK_EBASE, "Socket base error." },
    { ECode::SOCK_LINKDOWN, "Socket link down." },
    { ECode::SOCK_EADDRESS, "Bad socket address." },
};

}
