#ifndef __PROCESS_POOL_WORKER_H__
#define __PROCESS_POOL_WORKER_H__

#include <string>
#include <functional>

#include "io_select_item.h"
#include "sock_address.h"
#include "sock_client.h"

#include "process_return.h"
#include "process_log.h"
#include "process_id.h"

namespace infra::process::pool {

class ProcessPoolWorker {
public:
    typedef struct _Info {
        ProcessID pid_;
    } Info;

    enum class State {
        Initialize,
        Ready,
        Listening,
        Error,
    };

    class SelectItem : public io::SelectItem {
    public:
        SelectItem() : io::SelectItem() { }
        SelectItem(ProcessPoolWorker* worker) : io::SelectItem(worker->GetWorkerFD()) {
            worker_ = worker;
        }
        SelectItem(SelectItem& other): io::SelectItem(other) {
            worker_ = other.worker_;
            InputFunc = other.InputFunc;
            OutputFunc = other.OutputFunc;
            ErrorFunc = other.ErrorFunc;
        }
        ~SelectItem() { }

        SelectItem& operator=(SelectItem&& other) {
            io::SelectItem::operator=(other);
            worker_ = other.worker_;
            InputFunc = other.InputFunc;
            OutputFunc = other.OutputFunc;
            ErrorFunc = other.ErrorFunc;
            return *this;
        }

        io::IoRet Callback(int events) {
            ProcessRet ret = ProcessRet::SUCCESS;
            if (events & io::SelectEvent::Input) {
                if (InputFunc) {
                    ret = InputFunc(this);
                } else {
                    ret = _default_input_callback_func();
                }
                if (ret != ProcessRet::SUCCESS) {
                    return io::IoRet::IO_EINPUTCB;
                }
                events |= ~io::SelectEvent::Input;
            }
            if (events & io::SelectEvent::Output) {
                if (OutputFunc) {
                    ret = OutputFunc(this);
                    if (ret != ProcessRet::SUCCESS) {
                        return io::IoRet::IO_EOUTPUTCB;
                    }
                } else {
                }
                events |= ~io::SelectEvent::Output;
            }
            if (events & io::SelectEvent::Error) {
                if (ErrorFunc) {
                    ret = ErrorFunc(this);
                    if (ret != ProcessRet::SUCCESS) {
                        return io::IoRet::IO_EERRCB;
                    }
                } else {
                }
                events |= ~io::SelectEvent::Error;
            }
            return io::IoRet::SUCCESS;
        }

        std::function<ProcessRet(SelectItem*)> InputFunc;
        std::function<ProcessRet(SelectItem*)> OutputFunc;
        std::function<ProcessRet(SelectItem*)> ErrorFunc;
    private:
        ProcessPoolWorker* worker_;
        ProcessRet _default_input_callback_func() {
            printf("default callback\n");
            sock::SockFD* sockfd = dynamic_cast<sock::SockFD*>(event_.GetFdPointer());
            if (!sockfd) {
                PROCESS_ERROR("Listening file descriptor is NULL.");
                return ProcessRet::ERROR;
            }
            char buff[1024];
            memset(buff, 0, sizeof(buff));
            sockfd->Recv(NULL,buff,sizeof(buff));
            printf("recv %s\n", buff);
            return ProcessRet::SUCCESS;
        }
        ProcessRet _default_output_callback_func(SelectItem* select_item) {
            return ProcessRet::SUCCESS;
        }
        ProcessRet _default_error_callback_func(SelectItem* select_item) {
            return ProcessRet::SUCCESS;
        }
    };

public:
    ProcessPoolWorker();
    ProcessPoolWorker(ProcessID& pid, sock::SockFD& worker_fd);
    ProcessPoolWorker(sock::SockAddress& worker_address);
    ~ProcessPoolWorker();

    sock::SockFD& GetWorkerFD();
    SelectItem& GetSelectItem();
    State& GetState();

    bool IsReady();
    static io::Ret WorkerMessageListenerCallback(message::MessageListener* listener, io::SelectItemTemplate<MessageListener>* item);
private:
    Info info_;
    State state_;
    sock::SockFD worker_fd_;
    SelectItem select_item_;
private:
    ProcessPoolWorker(ProcessPoolWorker& other);
    const ProcessPoolWorker& operator=(const ProcessPoolWorker& other);

};

}

#endif
