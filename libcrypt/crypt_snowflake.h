/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2020-12-23 07:49
 * @file     : crypt_snowflake.h
 * @brief    : 
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __CRYPT_SNOWFLAKE_H__
#define __CRYPT_SNOWFLAKE_H__

#include <functional>

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

#include "object.h"
#include "crypt_log.h"
#include "crypt_return.h"

namespace infra::crypt {

/**
* @brief - Snowflake Unique serial number generator
*/
class Snowflake : virtual public infra::base::Object {
public:
    /**
    * @brief - Snowflake ID component definition
    */
    enum class Component : unsigned int {
        Begin = 0,
        Time,
        TimeBack,
        Mac,
        Ipv4,
        Ipv6,
        Serial,
        End,
    };

    /**
    * @brief - Pre-defined generation mode
    */
    enum class Mode {
        Default,
        SingleNode,
        Distribute,
    };
    using Composition_t = std::vector<std::tuple<Component, unsigned int>>;
public:
    /**
    * @brief Snowflake - Constructor Function
    *
    * @param [actime] - Activation time, if NULL, it will from Greenwich time.
    * @param [ifname] - Hardware interface name for ip or mac field.
    * @param [mode] - Pre-define composition mode.
    */
    Snowflake(struct timespec *actime, std::string ifname, Mode&& mode) :
            ifname_(ifname) {
        _gen_ipv6_bit(10);
        last_comp_value_ = comps1_;
        curr_comp_value_ = comps2_;
        if (actime) {
            actime_ = (actime->tv_sec * 1000) + (actime->tv_nsec / 1000000);
        } else {
            actime_ = 0;
        }
        for (auto it = Snowflake::ModeComposition.at(mode).begin();
                it != Snowflake::ModeComposition.at(mode).end(); it++) {
            composition_.push_back({std::get<0>(*it), std::get<1>(*it), (1 << std::get<1>(*it)) - 1});
        }
    }
    /**
    * @brief Snowflake - Constructor Function
    *
    * @param [actime] - Activation time, if NULL, it will from Greenwich time.
    * @param [ifname] - Hardware interface name for ip or mac field.
    * @param [composition] - User-define composition.
    */
    Snowflake(struct timespec *actime, std::string ifname, Composition_t&& composition) :
            ifname_(ifname) {
        last_comp_value_ = comps1_;
        curr_comp_value_ = comps2_;
        if (actime) {
            actime_ = (actime->tv_sec * 1000) + (actime->tv_nsec / 1000000);
        } else {
            actime_ = 0;
        }
        for (auto it : composition) {
            composition_.push_back({std::get<0>(it), std::get<1>(it), (1 << std::get<1>(it)) - 1});
        }
    }
    /**
    * @brief ~Snowflake - Destructor Function
    */
    ~Snowflake() { }

    /**
    * @brief ID - Generate ID
    *
    * @returns  ID
    */
    long ID() {
        long value = 0;
        for (auto it : composition_) {
            switch (std::get<0>(it)) {
                case Component::Time:
                    if (func_time_bit_) {
                        value = func_time_bit_(actime_, std::get<1>(it));
                    } else {
                        value = _gen_time_bit(std::get<1>(it));
                    }
                    if (value < 0) {
                        return INVALID_ID;
                    }
                    curr_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Time)] = value;
                    break;
                case Component::Mac:
                    if (func_mac_bit_) {
                        value = func_mac_bit_(std::get<1>(it));
                    } else {
                        value = _gen_mac_bit(std::get<1>(it));
                    }
                    if (value < 0) {
                        return INVALID_ID;
                    }
                    curr_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Mac)] = value;
                    break;
                case Component::Ipv4:
                    if (func_ipv4_bit_) {
                        value = func_ipv4_bit_(std::get<1>(it));
                    } else {
                        value = _gen_ipv4_bit(std::get<1>(it));
                    }
                    if (value < 0) {
                        return INVALID_ID;
                    }
                    curr_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Ipv4)] = value;
                    break;
                case Component::Ipv6:
                    if (func_ipv6_bit_) {
                        value = func_ipv6_bit_(std::get<1>(it));
                    } else {
                        value = _gen_ipv6_bit(std::get<1>(it));
                    }
                    if (value < 0) {
                        return INVALID_ID;
                    }
                    curr_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Ipv6)] = value;
                    break;
                case Component::Serial:
                case Component::TimeBack:
                default:
                    break;
            }
        }
        for (auto it : composition_) {
            switch (std::get<0>(it)) {
                case Component::Time:
                case Component::Mac:
                case Component::Ipv4:
                case Component::Ipv6:
                    break;
                case Component::Serial:
                    value = _gen_serial_bit(std::get<1>(it));
                    if (value < 0) {
                        return INVALID_ID;
                    }
                    curr_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Serial)] = value;
                    break;
                case Component::TimeBack:
                    value = _gen_timeback_bit(std::get<1>(it));
                    if (value < 0) {
                        return INVALID_ID;
                    }
                    curr_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::TimeBack)] = value;
                    break;
                default:
                    break;
            }
        }
        value = _assemble();
        std::swap(curr_comp_value_, last_comp_value_);
        return value;
    }
private:
    /**
    * @brief _gen_time_bit - Generation time bit
    *
    * @param [bit] - Time bits occupied size
    *
    * @returns  Time bit value
    */
    long _gen_time_bit(unsigned int bit) {
        long value = 0;
        struct timespec curr_time;
        memset(&curr_time, 0, sizeof(struct timespec));

        if (clock_gettime(CLOCK_REALTIME_COARSE, &curr_time) == -1) {
            Log::Error("clock_gettime Error, errno ", errno);
            return INVALID_ID;
        }
        value = (curr_time.tv_sec * 1000) + (curr_time.tv_nsec / 1000000);
        if (value < actime_) {
            Log::Error("The current time is before the activation time");
            return INVALID_ID;
        }
        value -= actime_;
        return (value & ((1l << bit) - 1));
    }

    long _gen_mac_bit(unsigned int bit) {
        long value = 0;
        if (!ifname_.size()) {
            Log::Error("Interface name is empty");
            return INVALID_ID;
        }
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd <= 0) {
            Log::Error("Socket Error errno ", errno);
            return INVALID_ID;
        }
        struct ifreq ifr;
        memset(&ifr, 0, sizeof(struct ifreq));
        strcpy(ifr.ifr_name, ifname_.c_str());
        if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
            Log::Error("Ioctl Error errno ", errno);
            close(fd);
            return INVALID_ID;
        }
        for(int i = 0; i < 6; i++){
            value |= ((ifr.ifr_hwaddr.sa_data[i] & 0xFFl) << ((5 - i) * 8));
        }
        close(fd);
        return (value & ((1l << bit) - 1));
    }

    long _gen_ipv4_bit(unsigned int bit) {
        long value = 0;
        if (!ifname_.size()) {
            Log::Error("Interface name is empty");
            return INVALID_ID;
        }
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd <= 0) {
            Log::Error("Socket Error errno ", errno);
            return INVALID_ID;
        }
        struct ifreq ifr;
        memset(&ifr, 0, sizeof(struct ifreq));
        strcpy(ifr.ifr_name, ifname_.c_str());
        if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
            Log::Error("Ioctl Error errno ", errno);
            close(fd);
            return INVALID_ID;
        }
        close(fd);
        value = ((struct sockaddr_in*)(&ifr.ifr_addr))->sin_addr.s_addr;
        return (value & ((1l << bit) - 1));
    }

    long _gen_ipv6_bit(unsigned int bit) {
        struct ifaddrs * pifaddr = nullptr;
        long value = 0;
        if (!ifname_.size()) {
            Log::Error("Interface name is empty");
            return INVALID_ID;
        }

        getifaddrs(&pifaddr);

        while (pifaddr != nullptr) {
            if (pifaddr->ifa_addr->sa_family == AF_INET6 &&
                    !strcmp(pifaddr->ifa_name, ifname_.c_str())) {
                struct in6_addr* addr6 = (struct in6_addr*)&((struct sockaddr_in *)pifaddr->ifa_addr)->sin_addr;
                //tmp = &((struct sockaddr_in *)pifaddr->ifa_addr)->sin_addr;
                value |= (0l | addr6->s6_addr[15]);
                value |= ((0l | addr6->s6_addr[14]) << 8 * 1);
                value |= ((0l | addr6->s6_addr[13]) << 8 * 2);
                value |= ((0l | addr6->s6_addr[12]) << 8 * 3);
                value |= ((0l | addr6->s6_addr[11]) << 8 * 4);
                value |= ((0l | addr6->s6_addr[10]) << 8 * 5);
                value |= ((0l | addr6->s6_addr[9]) << 8 * 6);
                value |= ((0l | addr6->s6_addr[8]) << 8 * 7);
                break;
            } 
            pifaddr = pifaddr->ifa_next;
        }
        if (!pifaddr) {
            Log::Error("Interface ", ifname_, " not found");
            return INVALID_ID;
        }
        return (value & ((1l << bit) - 1));
    }

    long _gen_serial_bit(unsigned int bit) {
        long value = 0;
        for (auto it : composition_) {
            if ((curr_comp_value_[static_cast<std::underlying_type<Component>::type>(std::get<0>(it))] !=
                    last_comp_value_[static_cast<std::underlying_type<Component>::type>(std::get<0>(it))]) &&
                    (std::get<0>(it) != Component::Serial) && (std::get<0>(it) != Component::TimeBack)) {
                value = last_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Serial)];
                return (value & ((1 << bit) - 1));
            }
        }
        value = last_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Serial)] + 1;
        return (value & ((1l << bit) - 1));
    }

    long _gen_timeback_bit(unsigned int bit) {
        long value = 0;
        if (curr_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Time)] < 
            last_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::Time)]) {
            value = last_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::TimeBack)] + 1;
        } else {
            value = last_comp_value_[static_cast<std::underlying_type<Component>::type>(Component::TimeBack)];
        }
        return (value & ((1l << bit) - 1));
    }

    long _assemble() {
        long value = 0;
        int bit_offset = 63;
        for (auto it : composition_) {
            bit_offset -= std::get<1>(it);
            value |= (curr_comp_value_[static_cast<std::underlying_type<Component>::type>(std::get<0>(it))]
                << bit_offset);
        }
        return value;
    }
    long _disassemble();
private:
    long actime_;
    std::string ifname_ = "";

    long* last_comp_value_ = nullptr;
    long* curr_comp_value_ = nullptr;
    long comps1_[static_cast<std::underlying_type<Component>::type>(Component::End)] = { 0 };
    long comps2_[static_cast<std::underlying_type<Component>::type>(Component::End)] = { 0 };
    std::vector<std::tuple<Component, unsigned int, long>> composition_;  // com, bit, max val

    std::function<long(time_t, unsigned int bit)> func_time_bit_  = nullptr;
    std::function<long(unsigned int bit)>         func_mac_bit_   = nullptr;
    std::function<long(unsigned int bit)>         func_ipv4_bit_  = nullptr;
    std::function<long(unsigned int bit)>         func_ipv6_bit_  = nullptr;

private:
    static const long INVALID_ID;
    static const std::map<Mode, std::vector<std::tuple<Component, unsigned int>>> ModeComposition;
};

const long Snowflake::INVALID_ID = 0x8000000000000000l;
const std::map<Snowflake::Mode, std::vector<std::tuple<Snowflake::Component, unsigned int>>> Snowflake::ModeComposition = {
    {Mode::Default,    {{ Snowflake::Component::Time, 41 },
                        { Snowflake::Component::Mac,   10 },
                        { Snowflake::Component::Serial,    12 }}},
    {Mode::SingleNode, {{ Snowflake::Component::Time, 45 },
                        { Snowflake::Component::TimeBack,  2  },
                        { Snowflake::Component::Serial,    16 }}},
    {Mode::Distribute, {{ Snowflake::Component::Time, 41 },
                        { Snowflake::Component::TimeBack,  2  },
                        { Snowflake::Component::Mac,   10 },
                        { Snowflake::Component::Serial,    12 }}},
};

}

#endif
