#ifndef __RING_LIST_H__
#define __RING_LIST_H__

namespace infra::container {

template <typename T>
class RingList {
public:
    RingList() {
        count_ = 0;
    }

private:
    unsigned int count_;
};

} //namespace end

#endif