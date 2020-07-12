#include "process_info.h"
#include "file.h"
#include "thread.h"
#include "report_mode.h"

#include <iostream>

int main()
{
    process::ProcessInfo* p = process::ProcessInfo::getInstance();
//    printf("pid %s name %s pname %s\n", p->GetPid().toString(), p->GetName().c_str(), p->GetRealName().c_str());
    std::cout << "pid " << p->GetPid() << " name " << p->GetName() << " pname " << p->GetRealName() << std::endl;

    thread::RegisterThread("grhtest");

    file::File fd(stdout);
    p->Report(fd,report::ReportMode::DETAIL);
    return 0;
}
