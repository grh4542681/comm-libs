#include "timer_time.h"
#include <time.h>

int main()
{
    infra::timer::Date d = infra::timer::Date::Now();
    std::cout << "Year " << d.Get(infra::timer::Date::Unit::Year) << std::endl;
    std::cout << "Month " << d.Get(infra::timer::Date::Unit::Month) << std::endl;
    std::cout << "DayOfMonth " << d.Get(infra::timer::Date::Unit::DayOfMonth) << std::endl;
    std::cout << "Week " << d.Get(infra::timer::Date::Unit::Week) << std::endl;
    std::cout << "DayOfWeek " << d.Get(infra::timer::Date::Unit::DayOfWeek) << std::endl;
    std::cout << "Hour " << d.Get(infra::timer::Date::Unit::Hour) << std::endl;
    std::cout << "Minute " << d.Get(infra::timer::Date::Unit::Minute) << std::endl;
    std::cout << "Second " << d.Get(infra::timer::Date::Unit::Second) << std::endl;

    std::cout << d.Format("%Y-%m-%d %H:%M:%S") << std::endl;
    std::cout << "--grh--" << d.date_.tm_isdst << std::endl;
    infra::timer::Time t(std::move(d));
    std::cout << t.GetSecond() << "---" << time(NULL) << " "<< t.Format("%Y-%m-%d %H:%M:%S") << std::endl;
    infra::timer::Date d2(t.GetSecond());
    std::cout << d2.Format("%Y-%m-%d %H:%M:%S") << std::endl;
    infra::timer::Date d3(time(NULL));
    std::cout << d3.Format("%Y-%m-%d %H:%M:%S") << std::endl;
}
