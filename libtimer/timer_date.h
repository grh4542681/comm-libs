#ifndef __TIMER_DATE_H__
#define __TIMER_DATE_H__

#include "object.h"

#include <time.h>

namespace infra::timer {

class Time;
class Date : virtual public base::Object {
public:
    friend class Time;
    enum class Unit {
        Year = 0,
        Month,
        Week,
        DayOfMonth,
        DayOfWeek,
        DayOfYear,
        Hour,
        Minute,
        Second,
    };

    enum class Month {
        January,
        February,
        March,
        April,
        May,
        June,
        July,
        August,
        September,
        October,
        November,
        December,
    };

    enum class Week {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday,
    };

    enum class Zodiac {
        Rat,
        Cow,
        Tiger,
        Rabbit,
        Dragon,
        Snake,
        Horse,
        Sheep,
        Monkey,
        Chicken,
        Dog,
        Pig,
    };
public:
    Date() {
        memset(&date_, 0, sizeof(struct tm));
    };
    Date(time_t sec) {
        gmtime_r(&sec, &date_);
        date_.tm_isdst = -1;
    }
    Date(const Date& other) {
        memcpy(&date_, &other.date_, sizeof(struct tm));
    }
    ~Date() { };

    const Date& operator=(const Date& other) {
        memcpy(&date_, &other.date_, sizeof(struct tm));
        return *this;
    }

#if 0
    Date& Increase(int value, const Unit& unit) {
    
    }

    Date& Decrease(int value, const Unit& unit) {

    }
#endif

    int Get(const Unit& unit) {
        int ret = 0;
        switch(unit) {
            case Unit::Year:
                ret = date_.tm_year + 1900;
                break;
            case Unit::Month:
                ret = date_.tm_mon + 1;
                break;
            case Unit::Week:
                ret = (date_.tm_yday / 7) + 1;
                break;
            case Unit::DayOfWeek:
                ret = date_.tm_wday + 1;
                break;
            case Unit::DayOfMonth:
                ret = date_.tm_mday;
                break;
            case Unit::DayOfYear:
                ret = date_.tm_yday + 1;
                break;
            case Unit::Hour:
                ret = date_.tm_hour;
                break;
            case Unit::Minute:
                ret = date_.tm_min;
                break;
            case Unit::Second:
                ret = date_.tm_sec;
                break;
            default:
                break;
        }
        return ret;
    }

    Date& Set(int value, const Unit& unit ) {
        if (value < 0) {
            return *this;
        }
        switch(unit) {
            case Unit::Year:
                date_.tm_year = value - 1900;
                break;
            case Unit::Month:
                if (value >= 12) {
                    return *this;
                }
                date_.tm_mon = value - 1;
                break;
            case Unit::Week:
                break;
            case Unit::DayOfWeek:
                if (value >= 7) {
                    return *this;
                }
                date_.tm_yday = (date_.tm_yday / 7) + value - 1;
                break;
            case Unit::DayOfMonth:
                if (value > 31) {
                    return *this;
                }
                date_.tm_mday = value;
                break;
            case Unit::DayOfYear:
                if (value >= 366) {
                    return *this;
                }
                date_.tm_yday = value - 1;
                break;
            case Unit::Hour:
                if (value >= 24) {
                    return *this;
                }
                date_.tm_hour = value;
                break;
            case Unit::Minute:
                if (value >= 60) {
                    return *this;
                }
                date_.tm_min = value;
                break;
            case Unit::Second:
                if (value >= 60) {
                    return *this;
                }
                date_.tm_sec = value;
                break;
            default:
                break;
        }
        return *this;
    }

    std::string Format(std::string format) {
        std::string str;
        char buff[1024];
        memset(buff, 0, sizeof(buff));

        if (!strftime(buff, sizeof(buff), format.c_str(), &date_)) {
            memset(buff, 0, sizeof(buff));
        }
        str.assign(buff);
        return str;
    }

    static Date Now() {
        time_t sec = time(NULL);
        return Date(gmtime(&sec));
    }
    struct tm date_;
private:
    Date(struct tm* date) {
        if (!date) {
            return;
        }
        std::cout << "--grh--" << date->tm_isdst << std::endl;
        memcpy(&date_, date, sizeof(struct tm));
        date_.tm_isdst = -1;
    }
};

}

#endif
