#ifndef __HEARTBEAR_CTRL_H__
#define __HEARTBEAR_CTRL_H__

namespace heartbeat {

class HeartbeatCtrl {
public:
    HeartbeatCtrl* getInstance();

private:
    HeartbeatCtrl();
    ~HeartbeatCtrl();

private:
    static HeartbeatCtrl* pInstance;

};

}

#endif
