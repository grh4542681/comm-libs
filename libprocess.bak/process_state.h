#ifndef __PROCESS_STATE_H__
#define __PROCESS_STATE_H__

namespace infra::process {

enum class ProcessState {
    Prepare,
    Normal,
    Exit,
};

}

#endif
