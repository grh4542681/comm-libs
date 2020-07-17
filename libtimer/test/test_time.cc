#include <unistd.h>
#include <string.h>
#include "timer_log.h"
#include "timer_return.h"
#include "timer_time.h"
#include "timer_date.h"

int main()
{
    infra::TimerLog::Debug("test log int = ", 5);
//    infra::TimeC::GetCurrTime().print();
    std::string now = infra::Time::Now().Format("%Y-%m-%d %H:%M:%S");
    printf("%s\n", now.c_str());
    struct timespec ts;
    memset(&ts, 0, sizeof(struct timespec));
    infra::Time t1 = infra::Time::Now();
    t1.To(&ts);
    printf("%ld, %ld\n",t1.GetSecond(),t1.GetNanoSecond());
    printf("%ld, %ld\n",ts.tv_sec,ts.tv_nsec);
    infra::Date d = infra::Date::Now();
    printf("year: %d\n", d.Get(infra::Date::Unit::Year));
    printf("mon: %d\n", d.Get(infra::Date::Unit::Month));
    printf("mday: %d\n", d.Get(infra::Date::Unit::DayOfMonth));
    printf("wday: %d\n", d.Get(infra::Date::Unit::DayOfWeek));
    printf("yday: %d\n", d.Get(infra::Date::Unit::DayOfYear));
    printf("week: %d\n", d.Get(infra::Date::Unit::Week));
    printf("hour: %d\n", d.Get(infra::Date::Unit::Hour));
    printf("min: %d\n", d.Get(infra::Date::Unit::Minute));
    printf("sec: %d\n", d.Get(infra::Date::Unit::Second));
    return 0;
}
