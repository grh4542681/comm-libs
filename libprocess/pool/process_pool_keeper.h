#ifndef __PROCESS_POOL_KEEPER_H__
#define __PROCESS_POOL_KEEPER_H__

#include <string>
#include <utility>
#include <tuple>
#include <map>

#include "process_id.h"
#include "process_pool_worker.h"
#include "process_pool_worker_template.h"

namespace process::pool {

template < typename F, typename ... Args >
class ProcessPoolKeeper {
public:
    ProcessPoolKeeper(std::string name, std::string config_filename, F worker, Args&& ... args) : tuple_args(std::forward_as_tuple(std::forward<Args>(args)...)){
        name_ = name;
        ptemp_ = ProcessPoolWorkerTemplate<F>(name_ + ":worker", config_filename, worker);
    }
    ~ProcessPoolKeeper() {

    }

    const size_t GetMaxSize() {
        return max_size_;
    }
    const size_t GetMinSize() {
        return min_size_;
    }

    ProcessRet Run() {
        _apply(std::make_index_sequence<std::tuple_size<std::tuple<Args...>>::value>());
        sleep(100);
        return ProcessRet::SUCCESS;
    }

    ProcessRet RunThread() {
        _apply(std::make_index_sequence<std::tuple_size<std::tuple<Args...>>::value>());
        return ProcessRet::SUCCESS;
    }

private:
    std::string name_;
    size_t min_size_;
    size_t max_size_;
    size_t cur_size_;
    bool init_flag_;
    bool auto_size_flag_;

    std::tuple<Args...> tuple_args;

    ProcessPoolWorkerTemplate<F> ptemp_;
    std::map<ProcessID, ProcessPoolWorker*> worker_;

    template <std::size_t... I>
    void _apply(std::index_sequence<I...>) {
        ptemp_.Run(std::get<I>(tuple_args)...);
    }

    ProcessRet _pool_init() {

    }

    void _pool_keeper() {

    }

};

}

#endif
