#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <functional>
#include "thread_template.h"

class T1 {
public:
//    void run(std::string a) {
//        printf("T1 [%s]\n", a.c_str());
//    }
    void run(int a) {
        printf("T1 [%d]\n", a);
    }
};
static void T3(std::string a) {
    printf("T3 [%s]\n", a.c_str());
}

int main() {
    T1 t1;
//    T2 t2;
//
    std::string s = "asd";
    int i = 1;
    infra::thread::TemplateMemFunction<T1, decltype(&T1::run)> T1run(&t1,&T1::run);
    auto h = T1run.Run(std::move(i));
    auto h2 = T1run.Run(std::move(i));
    std::cout << std::get<1>(h)->GetTid() << std::endl;
    std::cout << std::get<1>(h2)->GetTid() << std::endl;
    sleep(5);
    printf("main [%d]\n", std::get<1>(h)->GetTid().GetInterID());
    printf("main [%p]\n", std::get<1>(h));
    printf("main [%d]\n", std::get<1>(h2)->GetTid().GetInterID());
    printf("main [%p]\n", std::get<1>(h2));
    sleep(5);
    return 0;
//    t2.Run();
//    infra::thread::TemplateFunction<decltype(&T3)> T3run(&T3);
//    T3run.Run("asd");
//    return 0;
}
