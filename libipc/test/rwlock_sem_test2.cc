#include <unistd.h>
#include <stdio.h>
#include "sem/sem_rw_lock.h"

int main() {
    ipc::sem::SemRWLock rwlock("rwlock_test");
    rwlock.Create(0666);
    sleep(100);
    rwlock.Destroy();

    return 0;
}
