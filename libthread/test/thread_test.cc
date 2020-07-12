#include <unistd.h>
#include <stdio.h>

#include "thread_info.h"
#include "thread_template.h"

#include "process_info.h"
#include "file.h"
#include "report_mode.h"

int test_thread(int i, int b)
{
    printf("%d\n",i+b);
    sleep(10);
    return (i+b);
}

int main()
{
    auto thread_type = test_thread;

    thread::ThreadTemplate<void, decltype(thread_type), int> thread1(test_thread);
    thread::ThreadTemplate<void, decltype(thread_type), int> thread2(test_thread);
    thread::ThreadTemplate<void, decltype(thread_type), int> thread3(test_thread);

    process::ProcessInfo* p = process::ProcessInfo::getInstance();
    file::File fd(stdout);

    thread1.Run(100,1);
    sleep(1);
    p->Report(fd,report::ReportMode::DETAIL);
    sleep(1);
    thread2.Run(111,2);
    sleep(1);
    p->Report(fd,report::ReportMode::DETAIL);
    sleep(1);
    thread3.Run(122,3);
    sleep(1);
    p->Report(fd,report::ReportMode::DETAIL);

    thread1.Join();
    sleep(1);
    p->Report(fd,report::ReportMode::DETAIL);
    sleep(1);
    thread2.Join();
    sleep(1);
    p->Report(fd,report::ReportMode::DETAIL);
    sleep(1);
    thread3.Join();
    sleep(1);
    p->Report(fd,report::ReportMode::DETAIL);
    printf("return : %d\n",thread2.Return());
    printf("return : %d\n",thread3.Return());
    return 0;
}
