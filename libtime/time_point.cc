#include "time_point.h"

namespace infra::time {

Point::Point()
{
    std::chrono::time_point<std::chrono::system_clock> epoch(std::chrono::nanoseconds(0));
    date_ = std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(epoch)};
    week_ = std::chrono::year_month_weekday{std::chrono::floor<std::chrono::days>(epoch)};

    hours_ = std::chrono::floor<std::chrono::hours>(epoch) - std::chrono::floor<std::chrono::days>(epoch);
    minutes_ = std::chrono::floor<std::chrono::minutes>(epoch) - std::chrono::floor<std::chrono::hours>(epoch);
    seconds_ = std::chrono::floor<std::chrono::seconds>(epoch) - std::chrono::floor<std::chrono::minutes>(epoch);
    milliseconds_ = std::chrono::floor<std::chrono::milliseconds>(epoch) - std::chrono::floor<std::chrono::seconds>(epoch);
    microseconds_ = std::chrono::floor<std::chrono::microseconds>(epoch) - std::chrono::floor<std::chrono::milliseconds>(epoch);
    nanoseconds_ = std::chrono::floor<std::chrono::nanoseconds>(epoch) - std::chrono::floor<std::chrono::microseconds>(epoch);
}

Point::Point(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>&& time)
{
    date_ = std::chrono::year_month_day{std::chrono::floor<std::chrono::days>(time)};
    week_ = std::chrono::year_month_weekday{std::chrono::floor<std::chrono::days>(time)};

    hours_ = std::chrono::floor<std::chrono::hours>(time) - std::chrono::floor<std::chrono::days>(time);
    minutes_ = std::chrono::floor<std::chrono::minutes>(time) - std::chrono::floor<std::chrono::hours>(time);
    seconds_ = std::chrono::floor<std::chrono::seconds>(time) - std::chrono::floor<std::chrono::minutes>(time);
    milliseconds_ = std::chrono::floor<std::chrono::milliseconds>(time) - std::chrono::floor<std::chrono::seconds>(time);
    microseconds_ = std::chrono::floor<std::chrono::microseconds>(time) - std::chrono::floor<std::chrono::milliseconds>(time);
    nanoseconds_ = std::chrono::floor<std::chrono::nanoseconds>(time) - std::chrono::floor<std::chrono::microseconds>(time);
}

Point::~Point()
{

}

Point& Point::operator=(const Point& other)
{
    date_ = other.date_;
    week_ = other.week_;
    hours_ = other.hours_;
    minutes_ = other.minutes_;
    seconds_ = other.seconds_;
    milliseconds_ = other.milliseconds_;
    microseconds_ = other.microseconds_;
    nanoseconds_ = other.nanoseconds_;
    return *this;
}

Point& Point::operator+(const Duration& duration)
{
    return *this;
}

Point& Point::operator-(const Duration& duration)
{
    return *this;
}

Return Point::SetTime(Unit&& unit, long int value)
{
    Return ret = Return::SUCCESS;
    switch(unit) {
        case Unit::Nanosecond:
            if (value > 999 || value < 0)
                ret = Return::TIME_INVALID;
            nanoseconds_ = std::chrono::nanoseconds{value};
            break;
        case Unit::Microsecond:
            if (value > 999 || value < 0)
                ret = Return::TIME_INVALID;
            microseconds_ = std::chrono::microseconds{value};
            break;
        case Unit::Millisecond:
            if (value > 999 || value < 0)
                ret = Return::TIME_INVALID;
            milliseconds_ = std::chrono::milliseconds{value};
            break;
        case Unit::Second:
            if (value > 59 || value < 0)
                ret = Return::TIME_INVALID;
            seconds_ = std::chrono::seconds{value};
            break;
        case Unit::Minute:
            if (value > 59 || value < 0)
                ret = Return::TIME_INVALID;
            minutes_ = std::chrono::minutes{value};
            break;
        case Unit::Hour:
            if (value > 23 || value < 0)
                ret = Return::TIME_INVALID;
            hours_ = std::chrono::hours{value};
            break;
        case Unit::DayOfWeek:
            week_ = std::chrono::sys_days{week_.year()/week_.month()/
                    std::chrono::weekday_indexed{std::chrono::weekday(static_cast<unsigned int>(value)), week_.index() + 1}};
            date_ = std::chrono::year_month_day{week_};
            break;
        case Unit::Week:
            date_ = std::chrono::sys_days{date_.year()/1/(GetTime(Unit::DayOfYear) + ((value - GetTime(Unit::Week)) * 7))};
            week_ = std::chrono::year_month_weekday{date_};
            break;
        case Unit::DayOfMonth:
            date_ = std::chrono::sys_days{date_.year()/date_.month()/std::chrono::day{static_cast<unsigned int>(value)}};
            week_ = std::chrono::year_month_weekday{date_};
            break;
        case Unit::Month:
            date_ = std::chrono::sys_days{date_.year()/std::chrono::month{static_cast<unsigned int>(value)}/date_.day()};
            week_ = std::chrono::year_month_weekday{date_};
            break;
        case Unit::DayOfYear:
            date_ = std::chrono::sys_days{date_.year()/1/std::chrono::day(static_cast<unsigned int>(value))};
            week_ = std::chrono::year_month_weekday{date_};
            break;
        case Unit::Year:
            date_ = std::chrono::sys_days{std::chrono::year{static_cast<int>(value)}/date_.month()/date_.day()};
            week_ = std::chrono::year_month_weekday{date_};
            break;
    }
    return ret;
}

unsigned int Point::GetTime(Unit&& unit)
{
    switch(unit) {
        case Unit::Nanosecond:
            return nanoseconds_.count();
        case Unit::Microsecond:
            return microseconds_.count();
        case Unit::Millisecond:
            return milliseconds_.count();
        case Unit::Second:
            return seconds_.count();
        case Unit::Minute:
            return minutes_.count();
        case Unit::Hour:
            return hours_.count();
        case Unit::DayOfWeek:
            return week_.weekday().c_encoding();
        case Unit::Week:
            return (((std::chrono::sys_days){date_}) - std::chrono::sys_days{date_.year()/1/1}).count() / 7 + 1;
        case Unit::DayOfMonth:
            return (unsigned)(date_.day());
        case Unit::Month:
            return (unsigned)(date_.month());
        case Unit::DayOfYear:
            return (((std::chrono::sys_days){date_}) - std::chrono::sys_days{date_.year()/1/1}).count() + 1;
        case Unit::Year:
            return (unsigned)(int)(date_.year());
            break;
    }
    return 0;
}

std::string Point::Format(std::string format)
{
    std::string str;
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    
    std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::sys_days(date_) + seconds_ + minutes_ + hours_;
        
    time_t tt = std::chrono::system_clock::to_time_t(tp);

    if (!strftime(buff, sizeof(buff), format.c_str(), gmtime(&tt))) {
        memset(buff, 0, sizeof(buff));
    }
    str.assign(buff);
    return str;
}

Point Point::Now()
{
    return Point(std::chrono::system_clock::now());
}

}
