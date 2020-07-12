#ifndef __IO_FD_TYPE_H__
#define __IO_FD_TYPE_H__

namespace io {

enum class FDType {
    Unknow,
    File,
    Socket,
    Event,
    Timer,
};

}

#endif
