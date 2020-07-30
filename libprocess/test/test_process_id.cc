#include "process_log.h"
#include "process_id.h"

int main() {
    infra::process::Log::Debug("process id : ", infra::process::ID::GetProcessID(), "parent id is : ", infra::process::ID::GetParentProcessID());
    return 0;
}
