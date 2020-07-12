#ifndef __THREAD_MQ_H__
#define __THREAD_MQ_H__

#include "rm_mempool.h"
#include "thread_return.h"

namespace thread{

class ThreadMQ {
public:
    ThreadMQ(unsigned int size);
    ~ThreadMQ();

    ThreadRet Set();
    ThreadRet Get();

private:
    unsigned int size_;
};

}//namespace thread end

#endif