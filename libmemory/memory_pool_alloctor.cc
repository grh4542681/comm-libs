#include <string>

#include "mempool_alloctor.h"

namespace mempool {

MempoolAlloctor::MempoolAlloctor() : base::Alloctor()
{
    object_name_ = "MempoolAlloctor";
    object_type_name_ = "MempoolAlloctor";
    object_describe_ = "Class for memory allocate&&free from mempool";

    mp_ = Mempool::getInstance();
}
MempoolAlloctor::~MempoolAlloctor()
{

}

void* MempoolAlloctor::_malloc(size_t size) {
    return mp_->Malloc(size);
}

void MempoolAlloctor::_free(void* ptr) {
    mp_->Free(ptr);
}

}
