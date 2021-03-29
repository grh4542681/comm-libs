#ifndef __TIMER_TASK_H__
#define __TIMER_TASK_H__

#include "timer_return.h"

namespace infra::timer {

class Task : virtual public infra::base::Object {
public:
    Task() { }
    virtual ~Task() { }

    virtual Return Run() { return Retuen::TIMER_TASK_STOP; }
}

}

#endif
