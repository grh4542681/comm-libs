#ifndef __LOG_DEVICE_H__
#define __LOG_DEVICE_H__

#include <iostream>

namespace log {

class LogDeviceInterface {
public:
    LogDeviceInterface() { }
    ~LogDeviceInterface() { }

    virtual size_t write(std::string& appname __attribute__ ((unused)), LogPriority&& priotity __attribute__ ((unused)), std::string logstr) {
        std::cout << logstr << std::endl;
        return logstr.size();
    }
};

class LogDevice {
public:
    ~LogDevice() { }

    static LogDevice& Instance() {
        static LogDevice instance;
        return instance;
    }

    template < typename I, typename ... Args > LogDevice& SetInterface(Args ... args) {
        if (log_dev_) {
            delete log_dev_;
        }
        log_dev_ = new I(std::forward<Args>(args)...);
        return *this;
    }

    LogDeviceInterface* GetInterface() {
        return log_dev_;
    }

private:
    LogDevice() {
        log_dev_ = new LogDeviceInterface();
    }
private:
    LogDeviceInterface* log_dev_;
};

}

#endif
