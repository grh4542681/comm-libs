#ifndef __TIME_POINT_H__
#define __TIME_POINT_H__

#include <chrono>

#include "object.h"

#include "time_return.h"
#include "time_measure.h"
#include "time_duration.h"

namespace infra::time {

class Point : virtual public ::infra::base::Object {
public:
    Point();
    Point(const Point& other);
    ~Point();

    Point& operator=(const Point& other);
    Point& operator+(const Duration& duration);
    Point& operator-(const Duration& duration);

    Return SetTime(Unit&& unit, long int value);
    unsigned int GetTime(Unit&& unit);

    std::string Format(std::string format);

    static Point Now();
private:
    Point(std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>&& time);

    std::chrono::year_month_day date_;
    std::chrono::year_month_weekday week_;

    std::chrono::hours hours_;
    std::chrono::minutes minutes_;
    std::chrono::seconds seconds_;
    std::chrono::milliseconds milliseconds_;
    std::chrono::microseconds microseconds_;
    std::chrono::nanoseconds nanoseconds_;
};

}

#endif
