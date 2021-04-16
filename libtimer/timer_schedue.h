#ifndef __TIMER_SCHEDUE_H__
#define __TIMER_SCHEDUE_H__

#include <string>

#include "object.h"

#include "timer_date.h"
#include "timer_return.h"

namespace infra::timer {

class Schedue : virtual public infra::base::Object {
public:
    class Rule {
    public:
        Rule() { }
        virtual ~Rule() { }

        virtual Return Next(Date&& curr, Date& next) = 0;
    };
public:
    template <typename R> Schedue(R&& rule) {
        rule_ = new R(rule);
    }
    template <typename R> Schedue(const Schedue&& other) {
        rule_ = new R(*other.rule_);
    }
    ~Schedue();

    bool Vaild();

    Return Next(Date& next);
    Return Next(Date&& curr, Date& next);
    Return Next(Time& next);
    Return Next(Time&& curr, Time& next);
private:
    Rule* rule_;
};

}

#endif
