#include "io_log.h"
#include "io_select.h"

namespace io {

Select::Select(unsigned int item_size) : item_size_(item_size)
{
    efd_ = epoll_create(item_size);
    if (efd_ == -1) {
        init_flag_ = false;
    } else {
        init_flag_ = true;
    }
}

Select::~Select()
{

}

IoRet Select::AddEvent(FD& fd, int events)
{
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(struct epoll_event));

    ep_event.events = events;
    ep_event.events |= EPOLLET;

    ep_event.data.fd = fd.GetFD();
    if (epoll_ctl(efd_, EPOLL_CTL_ADD, fd.GetFD(), &ep_event) == -1) {
        int tmp_errno = errno;
        IO_ERROR("Epoll add fd error %s", strerror(tmp_errno));
        return tmp_errno;
    }
    return IoRet::SUCCESS;
}

IoRet Select::AddEvent(SelectEvent& event)
{
    return AddEvent(event.GetFd(), event.GetEvents());
}

IoRet Select::ModEvent(FD& fd, int events)
{
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(struct epoll_event));

    ep_event.events = events;
    ep_event.events |= EPOLLET;

    ep_event.data.fd = fd.GetFD();
    if (epoll_ctl(efd_, EPOLL_CTL_MOD, fd.GetFD(), &ep_event) == -1) {
        int tmp_errno = errno;
        IO_ERROR("Epoll add fd error %s", strerror(tmp_errno));
        return tmp_errno;
    }
    return IoRet::SUCCESS;
}

IoRet Select::ModEvent(SelectEvent& event)
{
    return ModEvent(event.GetFd(), event.GetEvents());
}

IoRet Select::DelEvent(FD& fd)
{
    if (epoll_ctl(efd_, EPOLL_CTL_DEL, fd.GetFD(), NULL) == -1) {
        int tmp_errno = errno;
        IO_ERROR("Epoll add fd error %s", strerror(tmp_errno));
        return tmp_errno;
    }
    return IoRet::SUCCESS;
}

std::vector<SelectEvent> Select::Listen(timer::Time* overtime)
{
    return Listen(NULL, overtime);
}

std::vector<SelectEvent> Select::Listen(process::signal::ProcessSignalSet* sigmask, timer::Time* overtime)
{
    std::vector<SelectEvent> event_vec;

    if (!init_flag_) {
        return event_vec;
    }

    struct epoll_event rep_evts[item_size_];
    memset(rep_evts, 0, sizeof(epoll_event) * item_size_);

    sigset_t* set = sigmask ? sigmask->GetSigset() : NULL;
    int otime = overtime ? static_cast<int>(overtime->GetTime(timer::Time::Unit::Millisecond)) : -1;

    int fd_num = epoll_pwait(efd_, rep_evts, item_size_, otime, set);
    if (fd_num == -1) {
        return event_vec;
    }

    for (int loop = 0; loop < fd_num; ++loop) {
        event_vec.push_back(SelectEvent(FD(rep_evts[loop].data.fd), rep_evts[loop].events));
    }
    return event_vec;
}

}
