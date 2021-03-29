#ifndef __TIMER_WHEEL_HANDLER_H__
#define __TIMER_WHEEL_HANDLER_H__

namespace infra::timer {

class WheelHandler : virtual public infra::base::Object {
public:
    static WheelHandler& Instance();
private:
    WheelHandler();
    ~WheelHandler();

    static WheelHandler* pInstance;
};

}

#endif
