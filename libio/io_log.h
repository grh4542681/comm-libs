#ifndef __IO_LOG_H__
#define __IO_LOG_H__

#include "log_interface.h"

#define IO_LOG_FORMAT_DEFAULT LOG_FORMAT_DEFAULT

namespace io {

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
        app_name_ = "IO";
        rule_map_.clear();
        rule_map_.insert({
            {log::LogPriority::Emergency, IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Alert,     IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Critical,  IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Error,     IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Warning,   IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Notice,    IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Info,      IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Debug,     IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Debug2,    IO_LOG_FORMAT_DEFAULT},
            {log::LogPriority::Debug3,    IO_LOG_FORMAT_DEFAULT},
        });
    }
};

}

#endif
