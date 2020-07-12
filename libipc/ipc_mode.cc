#include "ipc_mode.h"

namespace ipc {

int operator|(IpcMode a, IpcMode b) {
    return ((int)a | (int)b);
}
int operator|(int a, IpcMode b) {
    return (a | (int)b);
}
int operator|(IpcMode a, int b) {
    return ((int)a | b); 
}
int operator&(IpcMode a, IpcMode b) {
    return ((int)a & (int)b);
}
int operator&(int a, IpcMode b) {
    return (a & (int)b);
}
int operator&(IpcMode a, int b) {
    return ((int)a & b); 
}
bool operator==(int a, IpcMode b) {
    return (a == (int)b);
}
bool operator==(IpcMode a, int b) {
    return ((int)a == b); 
}

}
