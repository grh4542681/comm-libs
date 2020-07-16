#ifndef __TIMER_LOG_H__
#define __TIMER_LOG_H__

#include "log_interface.h"

#define TIMER_LOG_FORMAT_DEFAULT LOG_FORMAT_DEFAULT

namespace timer {

class Logger : public log::LogInterface {
public:
    ~Logger() {
        log::LogInterface::~LogInterface();
    }

    static Logger& Instance() {
        static Logger instance;
        return instance;
    }

    template <typename ... Args> static void Emergency(Args ... args) {
        Logger::Instance().log::LogInterface::Emergency(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Alert(Args ... args) {
        Logger::Instance().log::LogInterface::Alert(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Critical(Args ... args) {
        Logger::Instance().log::LogInterface::Critical(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Error(Args ... args) {
        Logger::Instance().log::LogInterface::Error(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Warning(Args ... args) {
        Logger::Instance().log::LogInterface::Warning(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Notice(Args ... args) {
        Logger::Instance().log::LogInterface::Notice(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Info(Args ... args) {
        Logger::Instance().log::LogInterface::Info(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug(Args ... args) {
        Logger::Instance().log::LogInterface::Debug(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug2(Args ... args) {
        Logger::Instance().log::LogInterface::Debug2(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug3(Args ... args) {
        Logger::Instance().log::LogInterface::Debug3(std::forward<Args>(args)...);
    }
private:
    Logger() : log::LogInterface() {
        app_name_ = "TIMER";
        rule_map_.clear();
        rule_map_.insert({
            {log::LogPriority::Emergency, TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Alert,     TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Critical,  TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Error,     TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Warning,   TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Notice,    TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Info,      TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Debug,     TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Debug2,    TIMER_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Debug3,    TIMER_LOG_FORMAT_DEFAULT},
        });
    }
};

}

#endif
