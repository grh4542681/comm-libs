#ifndef __MEMORY_LOG_H__
#define __MEMORY_LOG_H__

#include "log_interface.h"

#define MEMORY_LOG_FORMAT_DEFAULT LOG_FORMAT_DEFAULT

namespace infra::memory {

class Log : virtual public base::Object, public log::Interface {
public:
    ~Log() {
        log::Interface::~Interface();
    }

    static Log& Instance() {
        static Log instance;
        return instance;
    }

    template <typename ... Args> static void Emergency(Args ... args) {
        Log::Instance().log::Interface::Emergency(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Alert(Args ... args) {
        Log::Instance().log::Interface::Alert(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Critical(Args ... args) {
        Log::Instance().log::Interface::Critical(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Error(Args ... args) {
        Log::Instance().log::Interface::Error(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Warning(Args ... args) {
        Log::Instance().log::Interface::Warning(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Notice(Args ... args) {
        Log::Instance().log::Interface::Notice(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Info(Args ... args) {
        Log::Instance().log::Interface::Info(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug(Args ... args) {
        Log::Instance().log::Interface::Debug(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug2(Args ... args) {
        Log::Instance().log::Interface::Debug2(std::forward<Args>(args)...);
    }
    template <typename ... Args> static void Debug3(Args ... args) {
        Log::Instance().log::Interface::Debug3(std::forward<Args>(args)...);
    }
private:
    Log() : log::Interface() {
        app_name_ = "MEMORY";
        rule_map_.clear();
        rule_map_.insert({
            {log::Priority::Emergency, MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Alert,     MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Critical,  MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Error,     MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Warning,   MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Notice,    MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Info,      MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Debug,     MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Debug2,    MEMORY_LOG_FORMAT_DEFAULT},
            {log::Priority::Debug3,    MEMORY_LOG_FORMAT_DEFAULT},
        });
    }
};

}

#endif
