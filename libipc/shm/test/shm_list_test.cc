#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ipc_mode.h"
#include "shm/shm_list.h"

class AAA {
public:
    int o;
};

int main()
{
    int* i ;
    ipc::shm::ShmList<ipc::shm::ShmPosix, int> shmlist("shm_list_test");
    shmlist.Create(5, 0666);
    shmlist.Open(ipc::IpcMode::READ_WRITE);
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    i = shmlist.PushHead(5);
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    printf("--%d--\n",*i);
    i = shmlist.PushTail(6);
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    printf("--%d--\n",*i);
    i = shmlist.PushTail(100);
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    printf("--%d--\n",*i);
    i = shmlist.PushHead(10);
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    printf("--%d--\n",*i);
    printf("--%p--\n",i);

    printf("***%d**%d*%d*%d\n", *shmlist.begin(),shmlist.begin().ptr->next,shmlist.begin().ptr->prev,shmlist.begin().ptr->index);

    for(auto n : shmlist) {
        printf("**%d**\n", n);
    }
    printf("=============================\n");
    shmlist.PopTail();
    for(auto n : shmlist) {
        printf("**%d**\n", n);
    }
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    printf("=============================\n");
    auto it = shmlist.begin();
    for (; it != shmlist.end(); it++) {
        if (*it == 5) {
            shmlist.Pop(it);
        }
    }
    for(auto n : shmlist) {
        printf("**%d**\n", n);
    }
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    printf("=============================\n");
    shmlist.Close();
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    shmlist.Open(ipc::IpcMode::READ_WRITE);
    printf("%s---%d----%d--%d\n",shmlist.GetObjectName().c_str(),shmlist.GetObjectSize(),shmlist.GetObjectMaxNumber(),shmlist.GetObjectCurNumber());
    printf("***%d**%d*%d*%d\n", *shmlist.begin(),shmlist.begin().ptr->next,shmlist.begin().ptr->prev,shmlist.begin().ptr->index);
    for(auto n : shmlist) {
        printf("**%d**\n", n);
    }
    printf("=============================\n");

    sleep(10);
    shmlist.Close();
    printf("=============================\n");
    shmlist.Destroy();
    return 0;
}
