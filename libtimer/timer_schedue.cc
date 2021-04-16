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

}
