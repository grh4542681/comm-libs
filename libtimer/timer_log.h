#ifndef __TIMER_LOG_H__
#define __TIMER_LOG_H__

#include "log_interface.h"

#define TIMER_LOG_FORMAT_DEFAULT LOG_FORMAT_DEFAULT

namespace infra {

class TimerLog : public LogInterface {
public:
    ~TimerLog() {
        LogInterface::~LogInterface();
    }

    static TimerLog& Instance() {
        static TimerLog instance;
        return instance;
    }

    template <typename ... Args> static void Emergency(Args ... args) {
        TimerLog::Instance().LogInterface::Emergency(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Alert(Args ... args) {
        TimerLog::Instance().LogInterface::Alert(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Critical(Args ... args) {
        TimerLog::Instance().LogInterface::Critical(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Error(Args ... args) {
        TimerLog::Instance().LogInterface::Error(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Warning(Args ... args) {
        TimerLog::Instance().LogInterface::Warning(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Notice(Args ... args) {
        TimerLog::Instance().LogInterface::Notice(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Info(Args ... args) {
        TimerLog::Instance().LogInterface::Info(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug(Args ... args) {
        TimerLog::Instance().LogInterface::Debug(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug2(Args ... args) {
        TimerLog::Instance().LogInterface::Debug2(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug3(Args ... args) {
        TimerLog::Instance().LogInterface::Debug3(std::forward<Args>(args)...);
    }
private:
    TimerLog() : LogInterface() {
        app_name_ = "TIMER";
        rule_map_.clear();
        rule_map_.insert({
            {LogPriority::Emergency, TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Alert,     TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Critical,  TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Error,     TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Warning,   TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Notice,    TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Info,      TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Debug,     TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Debug2,    TIMER_LOG_FORMAT_DEFAULT},
            {LogPriority::Debug3,    TIMER_LOG_FORMAT_DEFAULT},
        });
    }
};

}

#endif
