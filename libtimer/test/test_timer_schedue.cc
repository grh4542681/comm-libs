#include "timer_time.h"
#include "timer_schedue_crontab.h"
#include "timer_schedue.h"

int main()
{
    infra::timer::Schedue s(infra::timer::Crontab("* * 3,2,5 * * 8 */58 */40"));
    infra::timer::Date d;
    s.Next(d);
    std::cout << "now [" << infra::timer::Date::Now().Format("%Y-%m-%d %H:%M:%S") << "] next[" << d.Format("%Y-%m-%d %H:%M:%S") << "]" << std::endl;
    return 0;
}
