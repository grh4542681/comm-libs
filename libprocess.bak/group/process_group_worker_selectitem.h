#ifndef __PROCESS_GROUP_WORKER_SELECTITEM_H__
#define __PROCESS_GROUP_WORKER_SELECTITEM_H__

#include "io_select_item_template.h"
#include "sock_fd.h"
#include "process_id.h"

namespace process::group {

template < typename HOST >
class ProcessGroupWorkerSelectItem : public io::SelectItemTemplate<HOST> {
public:
    ProcessGroupWorkerSelectItem() : io::SelectItemTemplate<HOST>() { }
    ProcessGroupWorkerSelectItem(HOST* host, const ProcessID& pid, const sock::SockFD& fd,
                        typename io::SelectItemTemplate<HOST>::CallbackFunc callback = nullptr) : io::SelectItemTemplate<HOST>(host, fd, callback) {
        pid_ = pid;
    }
    ProcessGroupWorkerSelectItem(ProcessGroupWorkerSelectItem& other) : io::SelectItemTemplate<HOST>(other) {
        pid_ = other.pid_;
    }
    ~ProcessGroupWorkerSelectItem() { }

    ProcessGroupWorkerSelectItem& operator=(ProcessGroupWorkerSelectItem& other) {
        io::SelectItemTemplate<HOST>::operator=(other);
        pid_ = other.pid_;
        return *this;
    }

    ProcessID& GetPid() {
        return pid_;
    }
private:
    ProcessID pid_;
};

}

#endif
