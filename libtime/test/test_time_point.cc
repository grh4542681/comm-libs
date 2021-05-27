#include <unistd.h>
#include <string.h>
#include "time_log.h"
#include "time_return.h"
#include "time_point.h"

int main()
{
    infra::time::Point curr = infra::time::Point::Now();
    std::string now = curr.Format("%Y-%m-%d %H:%M:%S %Z(%z) %u %W");
    printf("%s\n", now.c_str());
    printf("%d\n", curr.GetTime(infra::time::Unit::Week));
    printf("%d\n", curr.GetTime(infra::time::Unit::DayOfYear));
    curr.SetTime(infra::time::Unit::Nanosecond, 1l);
    curr.SetTime(infra::time::Unit::Second, 1l);
    curr.SetTime(infra::time::Unit::Hour, 1l);
//    curr.SetTime(infra::time::Unit::Year, 2000l);
//    curr.SetTime(infra::time::Unit::Month, 1l);
//    curr.SetTime(infra::time::Unit::DayOfMonth, 1l);
//    curr.SetTime(infra::time::Unit::DayOfYear, 134);
//    curr.SetTime(infra::time::Unit::Week, 21l);
    curr.SetTime(infra::time::Unit::DayOfWeek, 7);
    now = curr.Format("%Y-%m-%d %H:%M:%S %Z(%z) %u %W");
    printf("%s\n", now.c_str());
#if 0
    struct timespec ts;
    memset(&ts, 0, sizeof(struct timespec));
    infra::timer::Time t1 = infra::timer::Time::Now();
    t1.To(&ts);
    printf("%ld, %ld\n",t1.GetSecond(),t1.GetNanoSecond());
    printf("%ld, %ld\n",ts.tv_sec,ts.tv_nsec);
    infra::timer::Date d = infra::timer::Date::Now();
    printf("year: %d\n", d.Get(infra::timer::Date::Unit::Year));
    printf("mon: %d\n", d.Get(infra::timer::Date::Unit::Month));
    printf("mday: %d\n", d.Get(infra::timer::Date::Unit::DayOfMonth));
    printf("wday: %d\n", d.Get(infra::timer::Date::Unit::DayOfWeek));
    printf("yday: %d\n", d.Get(infra::timer::Date::Unit::DayOfYear));
    printf("week: %d\n", d.Get(infra::timer::Date::Unit::Week));
    printf("hour: %d\n", d.Get(infra::timer::Date::Unit::Hour));
    printf("min: %d\n", d.Get(infra::timer::Date::Unit::Minute));
    printf("sec: %d\n", d.Get(infra::timer::Date::Unit::Second));
#endif
    return 0;
}
