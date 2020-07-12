#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "mempool.h"
#include "container_return.h"

#define BITMAP_PAGESIZE (8)

namespace container {

class Bitmap {
public:
    Bitmap();
    Bitmap(size_t bit_size);
    Bitmap(size_t bit_size, void* bit_head);
    Bitmap(Bitmap& other);
    ~Bitmap();

    size_t GetMaxBitsize();
    size_t GetCurBitsize();

    ContainerRet Set(size_t index);
    ContainerRet Unset(size_t index);
    size_t Find0();
    size_t Find1();
    bool IsFull();

    static size_t GetBitmapSize(size_t bit_size);

    void Print1();
    void Print0();
    void Print();
private:
    mempool::Mempool* mempool_ = { mempool::Mempool::getInstance() };
    bool free_flag_ = { false };
    void* page_head_ = { NULL };
    size_t page_num_ = { 0 };
    size_t bit_max_size_ = { 0 };
    size_t bit_cur_size_ = { 0 };
};

}

#endif
