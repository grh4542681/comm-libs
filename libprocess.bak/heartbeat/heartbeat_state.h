#ifndef __HEARTBEAT_STATE_H__
#define __HEARTBEAT_STATE_H__

namespace infra::heartbeat {

enum class HeartbeatState {
    INVALID,
    HANDSHAKE,
    DISABLE,
    KEEP,
    LOST,
    DEAD,
};

}

#endif
