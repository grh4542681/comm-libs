#ifndef __TIME_MEASURE_H__
#define __TIME_MEASURE_H__

namespace infra::time {

enum class Unit : int {
    Nanosecond = 0,
    Microsecond,
    Millisecond,
    Second,
    Minute,
    Hour,
    DayOfWeek,
    Week,
    DayOfMonth,
    Month,
    DayOfYear,
    Year,
};

enum class Week : int {
    Monday = 0,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
};

enum class Month : int {
    January = 0,
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

enum class Zodiac : int {
    Rat = 0,
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

}

#endif
