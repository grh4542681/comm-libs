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
    ProcessHandler& handler = ProcessHandler::Instance();
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

    ProcessChild* child = parent->GetChildProcess(ProcessID(pid));
    if (child) {
        auto callback = child->GetDeadCallback();
        if (callback) {
            callback(NULL);
        }
        parent->DelChildProcess(ProcessID(pid));
        PROCESS_INFO("Remove child[%d] from [%d]", pid, parent_pid);
    } else {
        PROCESS_INFO("Dead process [%d] is not a child for [%d]", pid, parent_pid);
    }
}

}

#endif
