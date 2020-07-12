#include <stdio.h>
#include "link_list.h"
int main()
{
    container::LinkList<int> l;
    printf("count %d\n", l.count());
    l.pushback(1);
    l.pushback(2);
    l.pushback(3);
    l.pushback(4);
    l.pushback(5);
    printf("count %d\n", l.count());
    l.pushfront(6);
    l.pushfront(7);
    l.pushfront(8);
    l.pushfront(9);
    l.pushfront(10);
    printf("count %d\n", l.count());
    for (auto it : l) {
        printf("%d\n", it);
    }
    printf("=============================\n");
    for (auto it = l.begin(); it != l.end(); ++it){
        if (*it == 7 || *it == 8 || *it ==5 || *it == 10) {
            l.pop(it);
        }
    }
    for (auto it : l) {
        printf("%d\n", it);
    }
    printf("count %d\n", l.count());
    printf("=============================\n");
    l.popfront();
    l.popback();
    for (auto it : l) {
        printf("%d\n", it);
    }
    printf("count %d\n", l.count());
    printf("=======copy======================\n");
    container::LinkList<int> ll(l);
    for (auto it : ll) {
        printf("%d\n", it);
    }
    printf("count %d\n", ll.count());
    printf("===== oper= =======================\n");
    container::LinkList<int> lll = l;
    for (auto it : lll) {
        printf("%d\n", it);
    }
    printf("count %d\n", lll.count());
    printf("=============================\n");
    l.clear();
    printf("count %d\n", l.count());
    struct aaa {
        int i;
        char a;
    };
    struct aaa test1 = { 1,'1'};
    container::LinkList<struct aaa> l2;
    l2.pushback(test1);
    printf("count %d\n", l2.count());
    for (auto it : l2) {
        printf("%d\n", it.i);
    }

    return 0;
}
