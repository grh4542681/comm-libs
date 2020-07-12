#include "hash_table.h"
#include <stdio.h>

int main()
{
    auto lam = [](int x)->unsigned long{ return (unsigned long)x; };
    container::HashTable<int,decltype(lam)> ht(10, lam);
    ht.insert(4,3);
    printf("size %ld count %ld free %ld\n", ht.size(), ht.count(), ht.available());
    int* test = ht.find(3);
    printf("get value[%d]\n", *test);
    ht.insert(5,3);
    ht.insert(5,4);
    ht.insert(5,5);
    printf("get value[%d]\n", *test);
    ht.remove(3);
    printf("get value[%d]\n", *test);
    printf("size %ld count %ld free %ld\n", ht.size(), ht.count(), ht.available());
    ht.empty();
    printf("size %ld count %ld free %ld\n", ht.size(), ht.count(), ht.available());


    container::HashTable<int> ht2(25);
    printf("hash index[%ld]\n",ht2.hashcode("aa"));
    printf("hash index[%ld]\n",ht2.hashcode("ab"));
    printf("hash index[%ld]\n",ht2.hashcode("alksanefnb"));
    ht2.insert(100, "asda");
    printf("get value[%d]\n", *(ht2.find("asda")));
    printf("==========copy==========\n");
    container::HashTable<int> ht3(ht2);
    printf("hash index[%ld]\n",ht3.hashcode("aa"));
    printf("hash index[%ld]\n",ht3.hashcode("ab"));
    printf("hash index[%ld]\n",ht3.hashcode("alksanefnb"));
    ht3.insert(100, "asda");
    printf("get value[%d]\n", *(ht3.find("asda")));
    return 0;
}
