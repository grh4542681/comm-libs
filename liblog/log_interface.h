#ifndef __LOG_INTERFACE_H__
#define __LOG_INTERFACE_H__

#include <map>
#include <utility>
#include <chrono>
#include <thread>
#include <sstream>
#include <ctime>
#include <iomanip>

#include "log_priority.h"
#include "log_format.h"
#include "log_device.h"

namespace infra {

class LogInterface {
public:
    LogInterface() : log_device_(LogDevice::Instance()) { }
    virtual ~LogInterface() { }

    template <typename ... Args> void Emergency(Args&& ... args) {
        Log(LogPriority::Emergency, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Alert(Args&& ... args) {
        Log(LogPriority::Alert, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Critical(Args&& ... args) {
        Log(LogPriority::Critical, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Error(Args&& ... args) {
        Log(LogPriority::Error, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Warning(Args&& ... args) {
        Log(LogPriority::Warning, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Notice(Args&& ... args) {
        Log(LogPriority::Notice, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Info(Args&& ... args) {
        Log(LogPriority::Info, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Debug(Args&& ... args) {
        Log(LogPriority::Debug, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Debug2(Args&& ... args) {
        Log(LogPriority::Debug2, std::forward<Args>(args)...);
    }
    template <typename ... Args> void Debug3(Args&& ... args) {
        Log(LogPriority::Debug3, std::forward<Args>(args)...);
    }

    template <typename ... Args> void Log(LogPriority&& priority, Args&& ... args) {
        std::stringstream log_stream;
        _build_format(log_stream, std::move(priority));
        _build_log(log_stream, std::forward<Args>(args)...);
        log_device_.GetInterface()->write(app_name_, std::move(priority), log_stream.str());
    }

protected:
    void _build_priority(std::stringstream& log_stream, LogPriority&& priority) {
        log_stream << app_name_;
        switch (priority) {
            case LogPriority::Emergency:
                log_stream << "-EMERG";
                break;
            case LogPriority::Alert:
                log_stream << "-ALERT";
                break;
            case LogPriority::Critical:
                log_stream << "-CRIT";
                break;
            case LogPriority::Error:
                log_stream << "-ERR";
                break;
            case LogPriority::Warning:
                log_stream << "-WARN";
                break;
            case LogPriority::Notice:
                log_stream << "-NOTICE";
                break;
            case LogPriority::Info:
                log_stream << "-INFO";
                break;
            case LogPriority::Debug:
                log_stream << "-DEBUG";
                break;
            case LogPriority::Debug2:
                log_stream << "-DEBUG2";
                break;
            case LogPriority::Debug3:
                log_stream << "-DEBUG3";
                break;
            default:
                log_stream << "-UNKNOW";
                break;
        }
    }
    void _build_format(std::stringstream& log_stream, LogPriority&& priority) {

        auto timepoint = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
        std::time_t timestamp = std::chrono::system_clock::to_time_t(timepoint);
        std::tm* tm = std::localtime(&timestamp);

        LogFormat& format = format_default_;
        auto rule_map_it = rule_map_.find(priority);
        if (rule_map_it != rule_map_.end()) {
            format = rule_map_it->second;
        }

        for (auto format_it : format) {
            switch(format_it) {
                case LogFormat::Field::Pid:
                {
                    log_stream << std::this_thread::get_id();
                    break;
                }
                case LogFormat::Field::Tid:
                {
                    log_stream << std::this_thread::get_id();
                    break;
                }
                case LogFormat::Field::Function:
                {
                    log_stream << __func__;
                    break;
                }
                case LogFormat::Field::File:
                {
                    log_stream << __FILE__;
                    break;
                }
                case LogFormat::Field::LineNo:
                {
                    log_stream << __LINE__;
                    break;
                }
                case LogFormat::Field::TimeStamp:
                {
                    log_stream << timestamp;
                    break;
                }
                case LogFormat::Field::Year:
                {
                    log_stream << (tm->tm_year + 1900);
                    break;
                }
                case LogFormat::Field::Month:
                {
                    log_stream << std::setw(2) << std::setfill('0') << (tm->tm_mon + 1);
                    break;
                }
                case LogFormat::Field::DayOfMonth:
                {
                    log_stream << std::setw(2) << std::setfill('0') << tm->tm_mday;
                    break;
                }
                case LogFormat::Field::DayOfWeek:
                {
                    log_stream << std::setw(2) << std::setfill('0') << ((tm->tm_wday == 0) ? 7 : tm->tm_wday);
                    break;
                }
                case LogFormat::Field::Hour:
                {
                    log_stream << std::setw(2) << std::setfill('0') << tm->tm_hour;
                    break;
                }
                case LogFormat::Field::Minute:
                {
                    log_stream << std::setw(2) << std::setfill('0') << tm->tm_min;
                    break;
                }
                case LogFormat::Field::Second:
                {
                    log_stream << std::setw(2) << std::setfill('0') << tm->tm_sec;
                    break;
                }
                case LogFormat::Field::Millisecond:
                {
                    log_stream << std::setw(3) << std::setfill('0') << timepoint.time_since_epoch().count() % 1000000000 /1000000;
                    break;
                }
                case LogFormat::Field::Microsecond:
                {
                    log_stream << std::setw(6) << std::setfill('0') << timepoint.time_since_epoch().count() % 1000000000 /1000;
                    break;
                }
                case LogFormat::Field::Nanosecond:
                {
                    log_stream << std::setw(9) << std::setfill('0') << timepoint.time_since_epoch().count() % 1000000000;
                    break;
                }
                case LogFormat::Field::Blank:
                {
                    log_stream << " ";
                    break;
                }
                case LogFormat::Field::Dot:
                {
                    log_stream << ".";
                    break;
                }
                case LogFormat::Field::Colon:
                {
                    log_stream << ":";
                    break;
                }
                case LogFormat::Field::LeftBigBrackets:
                {
                    log_stream << "{";
                    break;
                }
                case LogFormat::Field::RightBigBrackets:
                {
                    log_stream << "}";
                    break;
                }
                case LogFormat::Field::LeftMidBrackets:
                {
                    log_stream << "[";
                    break;
                }
                case LogFormat::Field::RightMidBrackets:
                {
                    log_stream << "]";
                    break;
                }
                case LogFormat::Field::LeftSmallBrackets:
                {
                    log_stream << "(";
                    break;
                }
                case LogFormat::Field::RightSmallBrackets:
                {
                    log_stream << ")";
                    break;
                }
                case LogFormat::Field::HorizontalLine:
                {
                    log_stream << "-";
                    break;
                }
                case LogFormat::Field::VerticalLine:
                {
                    log_stream << "|";
                    break;
                }
                case LogFormat::Field::Logschema:
                {
                    _build_priority(log_stream, std::move(priority));
                    break;
                }
                default:
                    break;
            }
        }
    }

    template <typename T, typename ... Args> void _build_log(std::stringstream& log_stream, T&& arg, Args&& ... args) {
        log_stream << arg;
        if constexpr (sizeof...(args) > 0) {
            _build_log(log_stream, std::forward<Args>(args)...);
        }
    }
protected:
    std::string app_name_ = "Log";
    std::map<LogPriority, LogFormat> rule_map_ = {
        {LogPriority::Emergency, LOG_FORMAT_DEFAULT},
        {LogPriority::Alert,     LOG_FORMAT_DEFAULT},
        {LogPriority::Critical,  LOG_FORMAT_DEFAULT},
        {LogPriority::Error,     LOG_FORMAT_DEFAULT},
        {LogPriority::Warning,   LOG_FORMAT_DEFAULT},
        {LogPriority::Notice,    LOG_FORMAT_DEFAULT},
        {LogPriority::Info,      LOG_FORMAT_DEFAULT},
        {LogPriority::Debug,     LOG_FORMAT_DEFAULT},
        {LogPriority::Debug2,    LOG_FORMAT_DEFAULT},
        {LogPriority::Debug3,    LOG_FORMAT_DEFAULT},
    };
    LogDevice& log_device_;
    LogFormat format_default_ = LOG_FORMAT_DEFAULT;
};

}

#endif
