/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-16 04:40
 * @file     : timer_time.h
 * @brief    : Class of timer util.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __TIMER_TIME_H__
#define __TIMER_TIME_H__

#include <sys/time.h>
#include <cstring>
#include <string>

#include "object.h"

namespace infra::timer {

/**
* @brief - Virtual class Time
*/
class Time : virtual public base::Object {
public:
    enum class Unit {
        Second,
        Millisecond,
        Microsecond,
        Nanosecond,
    };
public:
    Time() {
        second_ = 0;
        nanosecond_ = 0;
    };
    Time(const Time& other) {
        second_ = other.second_;
        nanosecond_ = other.nanosecond_;
    }
    ~Time() { };

    const Time& operator= (const Time& other) {
        second_ = other.second_;
        nanosecond_ = other.nanosecond_;
        return *this;
    }
    const Time& operator= (const long nanosecond) {
        second_ = nanosecond/1000000000;
        nanosecond_ = nanosecond%1000000000;
        return *this;
    }

    Time operator+ (const Time& other) {
        Time new_timer;
        new_timer.second_ = second_ + other.second_;
        new_timer.nanosecond_ = nanosecond_ + other.nanosecond_;
        if (new_timer.nanosecond_ >= 1000000000) {
            new_timer.second_ += new_timer.nanosecond_/1000000000;
            new_timer.nanosecond_ %= 1000000000;
        }
        return new_timer;
    }
    Time operator+ (const long nanosecond) {
        Time new_timer;
        new_timer.second_ = second_ + (nanosecond/1000000000);
        new_timer.nanosecond_ = nanosecond_ + (nanosecond%1000000000);
        if (new_timer.nanosecond_ >= 1000000000) {
            new_timer.second_ += new_timer.nanosecond_/1000000000;
            new_timer.nanosecond_ %= 1000000000;
        }
        return new_timer;
    }

    Time operator- (const Time& other) {
        Time new_timer;
        new_timer.nanosecond_ = nanosecond_ - other.nanosecond_;
        new_timer.second_ = second_ - other.second_;
        if (new_timer.nanosecond_ < 0) {
            new_timer.second_ -= 1;
            new_timer.nanosecond_ += 1000000000;
        }
        return new_timer;
    }
    Time operator- (const long nanosecond) {
        Time new_timer;
        new_timer.nanosecond_ = nanosecond_ - (nanosecond%1000000000);
        new_timer.second_ = second_ - (nanosecond/1000000000);
        if (new_timer.nanosecond_ < 0) {
            new_timer.second_ -= 1;
            new_timer.nanosecond_ += 1000000000;
        }
        return new_timer;
    }
    const Time& operator+= (const Time& other) {
        second_ += other.second_;
        nanosecond_ += other.nanosecond_;
        if (nanosecond_ >= 1000000000) {
            nanosecond_ -= 1000000000;
            second_ += 1;
        }
        return *this;
    }
    const Time& operator+= (const long nanosecond) {
        second_ += nanosecond/1000000000;
        nanosecond_ += nanosecond%1000000000;
        if (nanosecond_ >= 1000000000) {
            nanosecond_ -= 1000000000;
            second_ += 1;
        }
        return *this;
    }
    const Time& operator-= (const Time& other) {
        second_ -= other.second_;
        nanosecond_ -= other.nanosecond_;
        if (nanosecond_ < 0) {
            nanosecond_ += 1000000000;
            second_ -= 1;
        }
        return *this;
    }
    const Time& operator-= (const long nanosecond) {
        second_ -= nanosecond/1000000000;
        nanosecond_ -= nanosecond%1000000000;
        if (nanosecond_ < 0) {
            nanosecond_ += 1000000000;
            second_ -= 1;
        }
        return *this;
    }

    bool operator== (const Time& other) {
        if ((second_ == other.second_) && (nanosecond_ == other.nanosecond_)) {
            return true;
        } else {
            return false;
        }
    }
    bool operator== (const long nanosecond) {
        if ((second_ == nanosecond/1000000000) && (nanosecond_ == nanosecond%1000000000)) {
            return true;
        } else {
            return false;
        }
    }

    bool operator< (const Time& other) {
        if (second_ == other.second_) {
            return (nanosecond_ < other.nanosecond_);
        } else {
            return (second_ < other.second_);
        }
    }

    Time& SetTime(long t, Unit unit) {
        second_ = 0;
        nanosecond_ = 0;
        switch (unit) {
            case Unit::Second:
                second_ = t;
                nanosecond_ = 0;
                break;
            case Unit::Millisecond:
                second_ = t / 1000;
                nanosecond_ = (t % 1000) * 1000000;
                break;
            case Unit::Microsecond:
                second_ = t/ 1000000;
                nanosecond_ = (t % 1000000) * 1000;
                break;
            case Unit::Nanosecond:
                second_ = t / 1000000000;
                nanosecond_ = (t % 1000000000);
                break;
            default:
                break;
        }
        return *this;
    }
    long GetTime(Unit unit) {
        long rtime = 0;
        switch (unit) {
            case Unit::Second:
                rtime = second_;
                break;
            case Unit::Millisecond:
                rtime = (second_*1000) + (nanosecond_/1000000);
                break;
            case Unit::Microsecond:
                rtime = (second_*1000000) + (nanosecond_/1000);
                break;
            case Unit::Nanosecond:
                rtime = (second_*1000000000) + nanosecond_;
                break;
            default:
                break;
        }
        return rtime;
    }

    Time& SetSecond(long second) {
        second_ = second;
        return *this;
    }

    Time& SetNanoSecond(long nanosecond) {
        nanosecond_ = nanosecond;
        if (nanosecond_ >= 1000000000) {
            second_ += nanosecond/1000000000;
            nanosecond_ %= 1000000000;
        }
        if (nanosecond_ < 0) {
            nanosecond_ += 1000000000;
            second_ -= 1;
        }
        return *this;
    }

    long GetSecond() {
        return second_;
    }

    long GetNanoSecond() {
        return nanosecond_;
    }

//    long ToSeconds() {
//        return second_;
//    }
//
//    long ToMilliseconds() {
//        return ((second_ * 1000) + (nanosecond_ / 1000000));
//    }
//
//    long ToMicrosecond() {
//        return ((second_ * 1000000) + (nanosecond_ / 1000));
//    }
//
//    long ToNanosecond() {
//        return ((second_ * 1000000000) + nanosecond_);
//    }

    bool IsZero() {
        return (!second_ && !nanosecond_);
    }

    /**
    * @brief Format - Format timer and convert to a string.
    *
    * @param [format] - format.
    * %a     The abbreviated weekday name according to the current locale.
    * %A     The full weekday name according to the current locale.
    * %b     The abbreviated month name according to the current locale.
    * %B     The full month name according to the current locale.
    * %c     The preferred date and timer representation for the current locale.
    * %C     The century number (year/100) as a 2-digit integer. (SU)
    * %d     The day of the month as a decimal number (range 01 to 31).
    * %D     Equivalent to %m/%d/%y.  (Yecch -- for Americans only.  Americans should note that in other countries %d/%m/%y is rather common.  This means that in international context  this
    *        format is ambiguous and should not be used.) (SU)
    * %e     Like %d, the day of the month as a decimal number, but a leading zero is replaced by a space. (SU)
    * %E     Modifier: use alternative format, see below. (SU)
    * %F     Equivalent to %Y-%m-%d (the ISO 8601 date format). (C99)
    * %G     The ISO 8601 week-based year (see NOTES) with century as a decimal number.  The 4-digit year corresponding to the ISO week number (see %V).  This has the same format and value
    *        as %Y, except that if the ISO week number belongs to the previous or next year, that year is used instead. (TZ)
    * %g     Like %G, but without century, that is, with a 2-digit year (00-99). (TZ)
    * %h     Equivalent to %b.  (SU)
    * %H     The hour as a decimal number using a 24-hour clock (range 00 to 23).
    * %I     The hour as a decimal number using a 12-hour clock (range 01 to 12).
    * %j     The day of the year as a decimal number (range 001 to 366).
    * %k     The hour (24-hour clock) as a decimal number (range 0 to 23); single digits are preceded by a blank.  (See also %H.)  (TZ)
    * %l     The hour (12-hour clock) as a decimal number (range 1 to 12); single digits are preceded by a blank.  (See also %I.)  (TZ)
    * %m     The month as a decimal number (range 01 to 12).
    * %M     The minute as a decimal number (range 00 to 59).
    * %n     A newline character. (SU)
    * %O     Modifier: use alternative format, see below. (SU)
    * %p     Either "AM" or "PM" according to the given timer value, or the corresponding strings for the current locale.  Noon is treated as "PM" and midnight as "AM".
    * %P     Like %p but in lowercase: "am" or "pm" or a corresponding string for the current locale. (GNU)
    * %r     The timer in a.m. or p.m. notation.  In the POSIX locale this is equivalent to %I:%M:%S %p.  (SU)
    * %R     The timer in 24-hour notation (%H:%M). (SU) For a version including the seconds, see %T below.
    * %s     The number of seconds since the Epoch, that is, since 1970-01-01 00:00:00 UTC. (TZ)
    * %S     The second as a decimal number (range 00 to 60).  (The range is up to 60 to allow for occasional leap seconds.)
    * %t     A tab character. (SU)
    * %T     The timer in 24-hour notation (%H:%M:%S). (SU)
    * %u     The day of the week as a decimal, range 1 to 7, Monday being 1.  See also %w.  (SU)
    * %U     The week number of the current year as a decimal number, range 00 to 53, starting with the first Sunday as the first day of week 01.  See also %V and %W.
    * %V     The ISO 8601 week number (see NOTES) of the current year as a decimal number, range 01 to 53, where week 1 is the first week that has at least 4 days in  the  new  year.   See
    *        also %U and %W.  (SU)
    * %w     The day of the week as a decimal, range 0 to 6, Sunday being 0.  See also %u.
    * %W     The week number of the current year as a decimal number, range 00 to 53, starting with the first Monday as the first day of week 01.
    * %x     The preferred date representation for the current locale without the timer.
    * %X     The preferred timer representation for the current locale without the date.
    * %y     The year as a decimal number without a century (range 00 to 99).
    * %Y     The year as a decimal number including the century.
    * %z     The timer-zone as hour offset from GMT.  Required to emit RFC 822-conformant dates (using "%a, %d %b %Y %H:%M:%S %z"). (GNU)
    * %Z     The timerzone or name or abbreviation.
    * %+     The date and timer in date(1) format. (TZ) (Not supported in glibc2.)
    * %%     A literal '%' character.
    *
    * @returns  string timer.
    */
    std::string Format(std::string format) {
        std::string str;
        char buff[1024];
        memset(buff, 0, sizeof(buff));

        if (!strftime(buff, sizeof(buff), format.c_str(), localtime(&second_))) {
            memset(buff, 0, sizeof(buff));
        }
        str.assign(buff);
        return str;
    }

    void To(struct timespec* p) {
        p->tv_sec = second_;
        p->tv_nsec = nanosecond_;
    }

    void To(struct timeval* p) {
        p->tv_sec = second_;
        p->tv_usec = nanosecond_ / 1000;
    }

    bool IsPositive() {
        return !(second_ < 0 && nanosecond_ < 0);
    }

    static Time Now() {
        Time now_time;
        struct timespec tp;
        if (clock_gettime(CLOCK_REALTIME, &tp) < 0) {
            return now_time;
        }
        return now_time.SetSecond(tp.tv_sec).SetNanoSecond(tp.tv_nsec);
    }

protected:
    long second_;        ///< seconds
    long nanosecond_;    ///< nanoseconds
};

}

#endif
