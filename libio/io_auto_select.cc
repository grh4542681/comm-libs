#include <utility>

#include "io_log.h"
#include "io_defines.h"
#include "io_auto_select.h"

namespace io {

AutoSelect::AutoSelect()
{
    fd_ = EpollFD(0);
    if (!fd_.Initalize()) {
        init_flag_ = false;
        IO_ERROR("Create epoll error");
    }
    init_flag_ = true;
}

AutoSelect::~AutoSelect()
{

}

bool AutoSelect::GetNonblock()
{
    return nonblock_flag_;
}

auto& AutoSelect::GetListenrThread()
{
    return listener_thread_;
}

IoRet AutoSelect::Listen(timer::Time overtime)
{
    return Listen(process::signal::ProcessSignalSet(), overtime);
}

IoRet AutoSelect::Listen(process::signal::ProcessSignalSet sigmask, timer::Time overtime)
{
    if (!init_flag_) {
        return IoRet::EINIT;
    }
    if (select_item_map_.empty()) {
        return IoRet::IO_ENOSELECTITEM;
    }
    return _select_listener_thread_handler(this, std::forward<process::signal::ProcessSignalSet>(sigmask), std::forward<timer::Time>(overtime));
}

IoRet AutoSelect::ListenThread(timer::Time overtime)
{
    return ListenThread(process::signal::ProcessSignalSet(), overtime);
}

IoRet AutoSelect::ListenThread(process::signal::ProcessSignalSet sigmask, timer::Time overtime)
{
    if (!init_flag_) {
        return IoRet::EINIT;
    }
    if (select_item_map_.empty()) {
        return IoRet::IO_ENOSELECTITEM;
    }
    listener_thread_ = thread::ThreadTemplate<void, decltype(&_select_listener_thread_handler), IoRet>(_select_listener_thread_handler);
    thread::ThreadRet ret = listener_thread_.Run(this, std::forward<process::signal::ProcessSignalSet>(sigmask), std::forward<timer::Time>(overtime));
    if (ret != thread::ThreadRet::SUCCESS) {
        IO_ERROR("Start io auto select thread error : %s", ret.Message().c_str());
        return IoRet::ETHREAD;
    }
    listener_thread_.Detach(); 
    return IoRet::SUCCESS;
}

IoRet AutoSelect::DelSelectItem(FD& fd)
{

}

IoRet AutoSelect::ModSelectItem()
{

}

const SelectItem* AutoSelect::GetSelectItem(FD& fd)
{
    auto it = select_item_map_.find(fd.GetFD());
    return (it == select_item_map_.end() ? nullptr : it->second);
}

bool AutoSelect::HasSelectItem(FD& fd)
{
    auto it = select_item_map_.find(fd.GetFD());
    return (it == select_item_map_.end() ? false : true);
}

IoRet AutoSelect::_select_item_traversal()
{
    return IoRet::SUCCESS;
}

IoRet AutoSelect::_select_listener_thread_handler(AutoSelect* instance, process::signal::ProcessSignalSet sigmask, timer::Time overtime)
{
    int item_size = 2048;
    struct epoll_event rep_evts[item_size];
    memset(rep_evts, 0, sizeof(epoll_event) * item_size);

    sigset_t* set = sigmask.IsEmpty() ? NULL : sigmask.GetSigset();
    int otime = instance->GetNonblock() ? 0 :
            (static_cast<int>(overtime.GetTime(timer::Time::Unit::Millisecond)) == 0 ? -1 : static_cast<int>(overtime.GetTime(timer::Time::Unit::Millisecond)));

    for (;;) {
        int fd_num = epoll_pwait(instance->fd_.GetFD(), rep_evts, item_size, otime, set);
        if (fd_num <= 0) {
            if (errno == EINTR || errno == 0) {
                IO_INFO("Epoll interrupted by a signal handler or time out");
                continue;
            } else {
                IO_ERROR("Epoll error : %s", strerror(errno));
                return errno;
            }
        }

        for (int loop = 0; loop < fd_num; ++loop) {
            auto it = instance->select_item_map_.find(rep_evts[loop].data.fd);
            if (it == instance->select_item_map_.end()) {
                IO_ERROR("Not found select item for file describetor [%d]", rep_evts[loop].data.fd);
                continue;
            }
            IoRet ret = it->second->Callback(rep_evts[loop].events);
            if (ret != IoRet::SUCCESS) {
                IO_ERROR("Execute callback for file describetor [%d] error.", rep_evts[loop].data.fd);
            }
            if (it->second->GetSelectEvent().HasOneshot()) {
                instance->select_item_map_.erase(it);
            }
        }
    }

    IO_INFO("Auto select terminal normal.");
    return IoRet::SUCCESS;
}

}
