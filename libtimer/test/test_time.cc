#include <unistd.h>
#include <string.h>
#include "timer_logger.h"
#include "timer_return.h"
#include "timer_time.h"
#include "timer_date.h"

int main()
{
    timer::Logger::Debug("test log int = ", 5);
//    timer::TimeC::GetCurrTime().print();
    std::string now = timer::Time::Now().Format("%Y-%m-%d %H:%M:%S");
    printf("%s\n", now.c_str());
    struct timespec ts;
    memset(&ts, 0, sizeof(struct timespec));
    timer::Time t1 = timer::Time::Now();
    t1.To(&ts);
    printf("%ld, %ld\n",t1.GetSecond(),t1.GetNanoSecond());
    printf("%ld, %ld\n",ts.tv_sec,ts.tv_nsec);
    timer::Date d = timer::Date::Now();
    printf("year: %d\n", d.Get(timer::Date::Unit::Year));
    printf("mon: %d\n", d.Get(timer::Date::Unit::Month));
    printf("mday: %d\n", d.Get(timer::Date::Unit::DayOfMonth));
    printf("wday: %d\n", d.Get(timer::Date::Unit::DayOfWeek));
    printf("yday: %d\n", d.Get(timer::Date::Unit::DayOfYear));
    printf("week: %d\n", d.Get(timer::Date::Unit::Week));
    printf("hour: %d\n", d.Get(timer::Date::Unit::Hour));
    printf("min: %d\n", d.Get(timer::Date::Unit::Minute));
    printf("sec: %d\n", d.Get(timer::Date::Unit::Second));
    return 0;
}
