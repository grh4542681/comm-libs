#include "time_duration.h"

namespace infra::time {

Duration::Duration() : precision_(Unit::Second), value_(0){ }
Duration::Duration(Unit&& unit) : precision_(unit), value_(0) { }
Duration::Duration(long value) : precision_(Unit::Second), value_(value) { }
Duration::Duration(Unit&& unit, long value) : precision_(unit), value(value) { }

Duration::Duration(const Duration& other)
{
    precision_ = other.precision_;
    value_ = other.value_;
}

Duration::~Duration() { }

Duration& Duration::operator=(const Duration& other)
{
    precision_ = other.precision_;
    value_ = other.value_;
    return *this;
}

Duration& Duration::operator+(const Duration& other)
{}

Duration& Duration::operator+(const long value)
{}

Duration& Duration::operator-(const Duration& other)
{}

Duration& Duration::operator-(const long value)
{}

Unit Duration::GetPrecision()
{}

Return Duration::SetPrecision()
{}


}
