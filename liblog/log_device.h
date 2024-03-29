#ifndef __LOG_DEVICE_H__
#define __LOG_DEVICE_H__

#include <iostream>

#include "object.h"

namespace infra::log {

class DeviceInterface : virtual public base::Object {
public:
    DeviceInterface() { }
    virtual ~DeviceInterface() { }

    virtual size_t write(std::string& appname __attribute__ ((unused)), Priority&& priotity __attribute__ ((unused)), const std::string& logstr) {
        std::cout << logstr << std::endl;
        return logstr.size();
    }
};

class Device : virtual public base::Object {
public:
    ~Device() {
        if (log_dev_) {
            delete log_dev_;
        }
    }

    static Device& Instance() {
        static Device instance;
        return instance;
    }

    template < typename I, typename ... Args > Device& SetInterface(Args ... args) {
        if (log_dev_) {
            delete log_dev_;
        }
        log_dev_ = new I(std::forward<Args>(args)...);
        return *this;
    }

    DeviceInterface* GetInterface() {
        return log_dev_;
    }

private:
    Device() {
        log_dev_ = new DeviceInterface();
    }
private:
    DeviceInterface* log_dev_;
};

}

#endif
