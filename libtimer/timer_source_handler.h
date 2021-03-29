#ifndef __TIMER_SOURCE_HANDLER_H__
#define __TIMER_SOURCE_HANDLER_H__

namespace infra::timer {

class SourceHandler : virtual public infra::base::Object {
public:
    static SourceHandler& Instance();
private:
    SourceHandler();
    ~SourceHandler();

    static SourceHandler* pInstance;
};

}

#endif
