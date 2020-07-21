#ifndef __LOG_PRIORITY_H__
#define __LOG_PRIORITY_H__

namespace infra::log {

enum class Priority : int {
    Emergency = 0,
    Alert,
    Critical,
    Error,
    Warning,
    Notice,
    Info,
    Debug,
    Debug2,
    Debug3,
};

}

#endif
