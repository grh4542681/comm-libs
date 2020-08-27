#include "temp_multiton.h"
#include <stdio.h>
int main(){
    int* a = odp::Multiton<int,int>::getInstance(1);
    printf("a[%d]\n",*a);
    a = odp::Multiton<int,int>::getInstance(2,5);
    printf("a[%d]\n",*a);
    a = odp::Multiton<int,int>::resetInstance(1,5);
    printf("a[%d]\n",*a);
    printf("size[%d]\n",odp::Multiton<int,int>::getSize());
    odp::Multiton<int,int>::delInstance(1);
    printf("size[%d]\n",odp::Multiton<int,int>::getSize());
    a = odp::Multiton<int,int>::getInstance(1,6);
    printf("a[%d]\n",*a);
    odp::Multiton<int,int>::delInstance(1);
    return 0;
}
