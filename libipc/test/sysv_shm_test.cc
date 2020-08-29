#include <unistd.h>
#include "ipc_mode.h"
#include "shm/shm_sysv.h"

int main()
{
    printf("%s\n", typeid(ipc::shm::ShmSysV).name());
    ipc::shm::ShmSysV shm("test");
    shm.Create(sizeof(int),0666);
    sleep(10);
    shm.Open(ipc::IpcMode::READ_WRITE);
    int* p = (int*)shm.GetHeadPtr();
    printf("---%p---\n",p);
    sleep(10);
    *p = 2;
    printf("---%p---\n",p);
    sleep(10);
    shm.Close();
    sleep(20);
    shm.Destroy();
    return 0;
}
