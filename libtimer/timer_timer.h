#ifndef __TIMER_TIMER_H__
#define __TIMER_TIMER_H__

#include <string>
#include <functional>

#include "io_auto_select.h"

#include "timer_return.h"
#include "timer_log.h"
#include "timer_fd.h"

namespace infra {

class Timer {
public:
    Timer();
    ~Timer();

private:
    Timer(Timer& other);
    const Timer& operator=(const Timer& other);

private:
    io::AutoSelect select_;

};

}

#endif
