#include "signal_id.h"

namespace infra::signal {

ID::ID()
{

}

ID::ID(int sig) : sig_(sig)
{

}

ID::ID(const ID& other)
{
    sig_ = other.sig_;
}

ID::~ID()
{

}

ID& ID::operator=(const ID& other)
{
    sig_ = other.sig_;
    return *this;
}

bool ID::operator<(const ID& other) const
{
    return (sig_ < other.sig_);
}

ID::ID_t ID::GetInterID() const
{
    return sig_;
}


}
