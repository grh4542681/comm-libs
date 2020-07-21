#ifndef __TIMER_DATE_H__
#define __TIMER_DATE_H__

#include <time.h>

namespace infra::timer {

class Date {
public:
    enum class Unit {
        Year,
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
                date_.tm_year = value;
                break;
            case Unit::Month:
                if (value >= 12) {
                    return *this;
                }
                date_.tm_mon = value;
                break;
            case Unit::Week:
                break;
            case Unit::DayOfWeek:
                if (value >= 7) {
                    return *this;
                }
                date_.tm_yday = (date_.tm_yday / 7) + value;
                break;
            case Unit::DayOfMonth:
                if (value > 31) {
                    return *this;
                }
                date_.tm_mon = value;
                break;
            case Unit::DayOfYear:
                if (value >= 366) {
                    return *this;
                }
                date_.tm_yday = value;
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

    static Date Now() {
        time_t sec = time(NULL);
        return Date(localtime(&sec));
    }
private:
    Date(struct tm* date) {
        if (!date) {
            return;
        }
        memcpy(&date_, date, sizeof(struct tm));
    }
    struct tm date_;
};

}

#endif
