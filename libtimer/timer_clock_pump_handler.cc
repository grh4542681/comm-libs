#include "io_select.h"
#include "io_select_event.h"

#include "timer_clock_pump_handler.h"

namespace infra::timer {

std::string ClockPumpHandler::ThreadName = "Timer clock pump thread";
ClockPumpHandler* ClockPumpHandler::pInstance = nullptr;

ClockPumpHandler::ClockPumpHandler() : thread_temp_(ThreadName ,this, &ClockPumpHandler::_thread_main)
{
}

ClockPumpHandler::~ClockPumpHandler()
{
}

Return ClockPumpHandler::AddPump(std::string name, Schedue&& schedue, Callback&& callback)
{
    return Return::SUCCESS;
}

Return ClockPumpHandler::UpdPump(std::string name, Schedue&& schedue, Callback&& callback)
{
    return Return::SUCCESS;
}

Return ClockPumpHandler::DelPump(std::string name)
{
    return Return::SUCCESS;
}

Return ClockPumpHandler::Start()
{
    return Return::SUCCESS;
}

Return ClockPumpHandler::Stop()
{
    return Return::SUCCESS;
}

Return ClockPumpHandler::_thread_main()
{
    infra::io::Select selector(1024);
    selector.AddEvent(thread_event_, infra::io::SelectEvent::Input);
    selector.Listen(nullptr);
    return Return::SUCCESS;
}

//static
ClockPumpHandler& ClockPumpHandler::Instance()
{
    if (!pInstance) {
        pInstance = new ClockPumpHandler();
    }
    return *pInstance;
}

}
