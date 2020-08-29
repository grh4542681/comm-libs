#include "sem/sem_sysv.h"
#include <stdio.h>
#include <unistd.h>
#include "timer_time.h"
int main()
{
    printf("hello\n");
    ipc::sem::SemSysV sem("test");
    sem.Create(3,0666);

    sem.V(1);
    printf("hello\n");
    sem.P(1,&(timer::Time().SetSecond(2)));
    printf("hello\n");
    sem.P(1,&(timer::Time().SetSecond(2)));
    printf("hello\n");
    sem.Destroy();
    printf("hello\n");
    return 0;
}
