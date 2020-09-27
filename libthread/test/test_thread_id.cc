#include "thread_log.h"
#include "thread_id.h"

int main() {
    infra::thread::Log::Debug("thread id : ", infra::thread::ID::GetThreadID());
    return 0;
}
