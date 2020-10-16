#ifndef __THREAD_STATE_H__
#define __THREAD_STATE_H__

namespace infra::thread {

enum class State {
    Invalid,
    Prepare,
    Running,
    RegisterFail,
    UnregisterFail,
    NormalExit,
};

}

#endif
