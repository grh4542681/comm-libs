#include "temp_singleton.h"
#include <stdio.h>
int main(){
    int* a = odp::Singleton<int>::getInstance();
    printf("a[%d]\n",*a);
    a = odp::Singleton<int>::getInstance(5);
    printf("a[%d]\n",*a);
    a = odp::Singleton<int>::resetInstance(5);
    printf("a[%d]\n",*a);
    odp::Singleton<int>::delInstance();
    return 0;
}
