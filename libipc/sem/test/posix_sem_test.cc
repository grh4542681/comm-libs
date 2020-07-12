#include "sem/sem_posix.h"
#include <stdio.h>
#include <unistd.h>
#include "timer_time.h"
int main()
{
    printf("hello\n");
    ipc::sem::SemPosix sem("test");
    sem.Create(3,0666);

//    sem.P(1,&(timer::Time().SetSecond(2)));
//    printf("hello\n");
    sem.V(1);
    sem.Close();
    sem.Open(ipc::IpcMode::READ_WRITE);
    printf("hello\n");
    sem.P(1,&(timer::Time().SetSecond(2)));
    printf("hello\n");
    printf("=========================\n");
//    sem.P(1,&(timer::Time().SetSecond(2)));
//    printf("hello\n");

    sleep(10);
    sem.Destroy();
    printf("hello\n");
    return 0;
}
