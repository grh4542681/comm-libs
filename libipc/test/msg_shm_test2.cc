#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ipc_mode.h"
#include "msg/msg_shm.h"

int main()
{
    ipc::msg::MsgShm ms("msgshm_test");
    ipc::IpcRet ret = ms.Open(ipc::IpcMode::READ_WRITE);
            printf("%d\n",ret.Code());
    char data[1024];

    while(1) {
        memset(data,0x00, sizeof(data));
        size_t ret = ms.Recv(data,sizeof(data),NULL);
        if (!ret) {
            printf("%d\n",(int)ms.GetRet().Code());
        }
        printf("--------------\n");
        
        sleep(2);
        printf("%s\n", data);
    }
    sleep(15);
    ms.Close();
    ms.Destroy();
    printf("--------------\n");
    return 0;
}
