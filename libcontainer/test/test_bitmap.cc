#include "container_bitmap.h"

int main()
{
    size_t i;
    infra::container::Bitmap bm(10);
    bm.Set(2);
    printf("%s\n",bm.Print().c_str());
    bm.Set(8);
    printf("%s\n",bm.Print().c_str());
    bm.Set(7);
    printf("%s\n",bm.Print().c_str());
    bm.Set(9);
    printf("%s\n",bm.Print().c_str());
    printf("%s\n",bm.Print1().c_str());
    printf("1---%d\n", bm.Find1());
    i = bm.Find1();
    bm.Unset(i);
    printf("%s\n",bm.Print().c_str());
    printf("1---%d\n", bm.Find1());
    printf("0---%d\n", bm.Find0());
    i = bm.Find0();
    bm.Set(i);
    printf("%s\n",bm.Print().c_str());
    printf("0---%d\n", bm.Find0());
    i = bm.Find0();
    bm.Set(i);
    printf("%s\n",bm.Print().c_str());
    printf("0---%d\n", bm.Find0());
    printf("---------------------------------------\n");
//    char cc;
//    infra::container::Bitmap bm2(8, (void*)&cc);
//    bm2.Print();
//    cc |= 1;
//    bm2.Print();
    return 0;
}
