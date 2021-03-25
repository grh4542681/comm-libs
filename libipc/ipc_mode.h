#ifndef __IPC_MODE_H__
#define __IPC_MODE_H__

namespace infra::ipc {

enum class IpcMode : int {
    READ_ONLY = 0x01,
    WRITE_ONLY = 0x02,
    READ_WRITE = 0x04,
    CREAT = 0x08,
    EXIST = 0x10, 
};

int operator|(IpcMode a, IpcMode b);
int operator|(int a, IpcMode b);
int operator|(IpcMode a, int b);
int operator&(IpcMode a, IpcMode b);
int operator&(int a, IpcMode b);
int operator&(IpcMode a, int b);
bool operator==(int a, IpcMode b);
bool operator==(IpcMode a, int b);

}

#endif
