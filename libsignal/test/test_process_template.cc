#include "process_signal.h"
#include "process_signal_common_callback.h"
#include "process_template.h"

#include <stdio.h>

void child_process()
{
    printf("child [%d]\n",getpid());
    infra::process::Handler& process_handler = infra::process::Handler::Instance();
    printf("child [%d] name [%s]\n", process_handler.GetPid().GetInterID(), process_handler.GetName().c_str());
    sleep(20);
}

void dead(int *status)
{
    printf("child [%d] dead\n",getpid());
}

int main (int argc, char** argv)
{
    infra::process::Handler& process_handler = infra::process::Handler::Instance();
    process_handler.SetCmdLine(argc, argv, environ);

    infra::process::Signal& signal_handler = infra::process::Signal::Instance();
    signal_handler.Register(infra::process::SignalId(SIGCHLD), std::move(infra::process::SignalAction().SetCallback(infra::process::SignalCommonCallback_SIGCHLD)));

    auto process_type = child_process;
    infra::process::Template<decltype(process_type)> p1("child", child_process);
    p1.SetDeadCallback(dead);
    infra::process::ID child;
    auto runret = p1.Run();
    child = std::get<const infra::process::ID>(runret);
    printf("--%d--\n", child.GetInterID());
    sleep(30);

    return 0;
}
