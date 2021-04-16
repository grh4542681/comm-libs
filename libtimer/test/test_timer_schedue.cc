#include "timer_schedue_cronrule.h"
#include "timer_schedue.h"

int main()
{
    infra::timer::Schedue s(infra::timer::CronRule("* * * * * * * */5"));
    return 0;
}
