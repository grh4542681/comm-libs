#ifndef __PROCESS_STATE_H__
#define __PROCESS_STATE_H__

namespace infra::process {

enum class State {
    Prepare,
    Normal,
    Exit,
};

}

#endif
