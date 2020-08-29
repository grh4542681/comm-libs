#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ipc_mode.h"
#include "msg/msg_shm.h"

int main()
{
    ipc::msg::MsgShm ms("msgshm_test");
    ms.Create(100,0777);
    ms.Open(ipc::IpcMode::READ_WRITE);

    printf("num %d  fsize %d\n", ms.GetMsgNum(),ms.GetFreeSize());
    sleep(5);
    char data[1024];

    int loop;
    for (loop = 0; loop < 20; loop++) {
        memset(data,0,sizeof(data));
        printf("input:");
        scanf("%s",data);
        ms.Send(data,strlen(data),&(timer::Time().SetSecond(2)));
        printf("num %d  fsize %d  head %d tail %d start %d end %d\n", ms.GetMsgNum(),ms.GetFreeSize(),ms.p_shm_head_->msg_head,ms.p_shm_head_->msg_tail,ms.p_shm_head_->data_start,ms.p_shm_head_->data_end);
        sleep(2);
    }
    sleep(100);
    ms.Close();
    ms.Destroy();
    return 0;
}
