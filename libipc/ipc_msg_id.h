#ifndef __MSG_ID_H__
#define __MSG_ID_H__

namespace infra::ipc::msg {

class MsgID {
public:
    MsgID() {
        id_ = 0;
    }
    MsgID(int id) {
        id = id_;
    }

    int GetID() {
        return id_;
    }
private:
    int id_;
};

}

#endif
