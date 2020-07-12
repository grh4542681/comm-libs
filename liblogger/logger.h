#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "allocator.h"

namespace log {

enum class Priority : int {
    Emergency = 0,
    Alert,
    Critical,
    Error,
    Warning,
    Notice,
    Info,
    Debug,
};

enum class Field : int {
    Pid,
    Tid,

    Funcname,
    Filename,
    Lineno,

    TimeStamp,
    TimeFormat,
}
class Logdev {
public:
    Logdev() {}
    virtual ~Logdev() {}

    static Logdev& Instance(base::Allocator&& alloc = base::Allocator()) {
        static Logdev* instance_ =  alloc.Allocate<Logdev>();
        return *instance_;
    }
    virtual record()
};

class Log {
public:
    Log(base::Allocator&& alloc = base::Allocator()) : alloc_(alloc) {
    }
    ~Log() {
    }

    template < typename Dev >
    static void InitializeDevice(base::Allocator&& alloc = base::Allocator()) {
        devInstance_ = Dev::Instance(alloc);
    }

    void Debug();
private:
    Allocator& alloc_;
    static Logdev* devInstance_;
    std::map<std::string, >
};

}

#endif
