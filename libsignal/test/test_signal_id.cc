#include <signal.h>

#include "signal_log.h"
#include "signal_id.h"

int main() {
    infra::signal::ID sig(SIGCHLD);
    infra::signal::Log::Debug("signal id : ", sig.GetInterID());
    return 0;
}
