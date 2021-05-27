#ifndef __TIME_DURATION_H__
#define __TIME_DURATION_H__

#include "object.h"

#include "time_return.h"
#include "time_measure.h"

namespace infra::time {

class Duration : virtual public ::infra::base::Object {
public:
    Duration();
    Duration(Unit&& unit);
    Duration(long value);
    Duration(Unit&& unit, long value);
    Duration(const Duration& other);
    ~Duration();

    Duration& operator=(const Duration& other);
    Duration& operator+(const Duration& other);
    Duration& operator+(const long value);
    Duration& operator-(const Duration& other);
    Duration& operator-(const long value);

    Unit GetPrecision();
    Return SetPrecision();

private:
    Unit precision_;
    long value_;
};

}

#endif
