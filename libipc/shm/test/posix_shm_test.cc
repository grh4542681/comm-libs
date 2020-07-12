#include <unistd.h>
#include "string.h"
#include "shm/shm_posix.h"
#include "ipc_mode.h"

int main()
{
    ipc::shm::ShmPosix shm("posix_shm_text");
    if (shm.Create(1024,0666) != ipc::IpcRet::SUCCESS) {
        printf("-----------\n");
        return 0;
    }
    sleep(10);
    shm.Open(ipc::IpcMode::READ_WRITE);
    void* p = shm.GetHeadPtr();
    printf("%p\n", p);
//    *(int*)p = 10;
//    printf("%d\n", *(int*)p);
//    memcpy((char*)p, "hello", 5);
//    printf("%s\n",(char*)p);
    shm.Sync();
    sleep(10);
    shm.Close();
    sleep(10);
    shm.Destroy();
    return 0;
}
