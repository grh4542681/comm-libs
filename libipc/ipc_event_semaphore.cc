#include "event_semaphore.h"

namespace infra::ipc::event {

SemEvent::SemEvent()
{
}

SemEvent::SemEvent(std::string name)
{
}

SemEvent::~SemEvent()
{
}

EventFD& SemEvent::GetEFD()
{
    return fd_;
}

bool SemEvent::IsExist()
{
    return true;
}

IpcRet SemEvent::Create([[maybe_unused]] size_t semnum, [[maybe_unused]] mode_t mode)
{
    return IpcRet::SUCCESS;
}

IpcRet SemEvent::Destroy()
{
    return IpcRet::SUCCESS;
}

IpcRet SemEvent::_p(size_t sem_index, timer::Time* overtime)
{
    return IpcRet::SUCCESS;
}

IpcRet SemEvent::_v(size_t sem_index)
{
    return IpcRet::SUCCESS;
}

}
