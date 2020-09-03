/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-17 10:34
 * @file     : process_signal_common_callback.h
 * @brief    : Default signal callbcak function.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PROCESS_SIGNAL_COMMON_CALLBACK_H__
#define __PROCESS_SIGNAL_COMMON_CALLBACK_H__

#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#include "process_handler.h"
#include "process_log.h"

namespace infra::process {

/**
* @brief SignalCommonCallback_SIGCHLD - Default callback function for SIGCHLD.
*                   If the dead process is current procces's children, the 
*                   dead process dead callback function that register into cur-
*                   rent process will be invoke.
*
* @param [sig] - signal
*/
void SignalCommonCallback_SIGCHLD(int sig) {
    Handler& handler = Handler::Instance();
    pid_t parent_pid = handler.GetPid().GetInterID();
    pid_t pid = waitpid(0, NULL, WNOHANG);
    if (pid == 0) {
        Log::Info("Process [", parent_pid, "] catch a SIGCHLD that not belong to it.");
        return;
    } else if (pid < 0) {
        Log::Info("Process [", parent_pid, "] catch a SIGCHLD, waitpid error. errno[", errno, "]");
        return;
    } else {
        Log::Info("Process [", parent_pid, "] catch a SIGCHLD from [", pid, "]");
    }

    auto child_ret = handler.GetChild(ID(pid));
    if (std::get<Return>(child_ret) == Return::SUCCESS) {
        auto callback = std::get<Child&>(child_ret).GetDeadCallback();
        if (callback) {
            callback(NULL);
        }
        handler.DelChild(ID(pid));
        Log::Info("Remove child[", pid, "] from [", parent_pid, "]");
    } else {
        Log::Info("Dead process [", pid, "] is not a child for [", parent_pid, "]");
    }
}

}

#endif
