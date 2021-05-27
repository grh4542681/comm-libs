#include "timer_log.h"
#include "timer_schedue.h"

namespace infra::timer {

Schedue::~Schedue()
{
    if (rule_) {
        delete rule_;
    }
}

bool Schedue::Vaild()
{
    return (rule_ != nullptr);
}

Return Schedue::Next(Date& next)
{
    return Next(Date::Now(), next);
}

Return Schedue::Next(Date&& curr, Date& next)
{
    return rule_->Next(std::move(curr), next);
}

Return Schedue::Next(Time& next)
{

}

Return Schedue::Next(Time&& curr, Time& next)
{

}

}
