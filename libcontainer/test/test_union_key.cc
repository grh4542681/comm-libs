#include "container_union_weak_key.h"
#include <map>
#include <stdio.h>
int main()
{
    std::map<infra::container::UnionWeakKey<int, int>, std::string> m;
    infra::container::UnionWeakKey<int, int> key(1,2);
    printf("%d\n", std::get<0>(key.GetTupleKey()));
//    m.insert(infra::container::UnionWeakKey<int, int>(1,1), "hello");
//    m.insert({key, "hello"});
    char aa[10];
    snprintf(aa, 10, "%s", "aaaaaaaaaaaaaaaaaaaaaaaaaaa");
}
