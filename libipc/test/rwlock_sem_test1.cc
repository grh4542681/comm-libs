#include <unistd.h>
#include <stdio.h>
#include "sem/sem_rw_lock.h"

int main() {
    ipc::sem::SemRWLock rwlock("rwlock_test");
    //rwlock.Create(0666);
    rwlock.Open();
    rwlock.RLock(NULL);
    rwlock.print();
    rwlock.UnLock(NULL);
    rwlock.print();
    rwlock.Close();
    //rwlock.Destroy();

    return 0;
}
