#ifndef __TIMER_HANDLER_H__
#define __TIMER_HANDLER_H__

#include <string>
#include <functional>

#include "object.h"
#include "io_auto_select.h"

#include "timer_return.h"
#include "timer_log.h"
#include "timer_fd.h"

namespace infra::timer {

class Handler : virtual public base::Object {
public:
    Handler();
    ~Handler();

private:
    Handler(Handler& other);
    const Handler& operator=(const Handler& other);

private:
    io::AutoSelect select_;

};

}

#endif
