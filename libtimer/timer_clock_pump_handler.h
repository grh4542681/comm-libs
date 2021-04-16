#ifndef __TIMER_CLOCK_PUMP_HANDLER_H__
#define __TIMER_CLOCK_PUMP_HANDLER_H__

#include <tuple>
#include <map>

#include "thread_mutex.h"
#include "thread_event.h"
#include "thread_template.h"

#include "timer_return.h"
#include "timer_schedue.h"

namespace infra::timer {

class ClockPumpHandler : virtual public infra::base::Object {
public:
    using Callback = void (*)(void);
public:
    static ClockPumpHandler& Instance();

    Return AddPump(std::string name, Schedue&& schedue, Callback&& callback);
    Return UpdPump(std::string name, Schedue&& schedue, Callback&& callback);
    Return DelPump(std::string name);

    Return Start();
    Return Stop();
public:
    static std::string ThreadName;

private:
    ClockPumpHandler();
    ~ClockPumpHandler();

    Return _thread_main();

private:
    infra::thread::Event thread_event_;
    std::map<std::string, std::tuple<Schedue, Callback>> schedue_map_;
    infra::thread::TemplateMemFunction<ClockPumpHandler, decltype(&ClockPumpHandler::_thread_main)> thread_temp_;;

    static ClockPumpHandler* pInstance;
};

}

#endif
