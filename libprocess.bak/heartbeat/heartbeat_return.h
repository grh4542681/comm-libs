#ifndef __HEARTBEAT_RETURN_H__
#define __HEARTBEAT_RETURN_H__

namespace infra::heartbeat{

enum class HeartBeatRet: int {
//common return val
    SUCCESS = 0x00,
    ERROR,
    EBADARGS,
    EMALLOC,
    EINIT,
    ETIMEOUT,
    ESUBCLASS,

    EUNKOWNERRNO,

};

}

#endif
