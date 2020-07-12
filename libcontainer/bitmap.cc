#include <string>
#include "bitmap.h"

namespace container {

Bitmap::Bitmap() { }

Bitmap::Bitmap(size_t bit_size)
{
    bit_max_size_ = bit_size;
    page_num_ = (bit_size + (BITMAP_PAGESIZE-1)) / BITMAP_PAGESIZE;
    page_head_ = mempool_->Malloc(page_num_ * BITMAP_PAGESIZE);
    if (page_head_) {
        free_flag_ = true;
    }
}

Bitmap::Bitmap(size_t bit_size, void* bit_head)
{
    bit_max_size_ = bit_size;
    page_num_ = (bit_size + (BITMAP_PAGESIZE-1)) / BITMAP_PAGESIZE;
    if (bit_head) {
        page_head_ = bit_head;
        free_flag_ = false;
    } else {
        page_head_ = mempool_->Malloc(page_num_ * BITMAP_PAGESIZE);
        if (page_head_) {
            free_flag_ = true;
        }
    }
}

Bitmap::Bitmap(Bitmap& other)
{

}

Bitmap::~Bitmap()
{
    if (free_flag_) {
        mempool_->Free(page_head_);
    }
}

size_t Bitmap::GetMaxBitsize()
{
    return bit_max_size_;
}

size_t Bitmap::GetCurBitsize()
{
    return bit_cur_size_;
}

ContainerRet Bitmap::Set(size_t index)
{
    if (!page_head_) {
        return ContainerRet::EINIT;
    }
    if (index == 0 || index > bit_max_size_) {
        return ContainerRet::BM_EOUTBOUND;
    }
    size_t page_index = (index + BITMAP_PAGESIZE - 1) / BITMAP_PAGESIZE;
    size_t page_offset = (index - 1 ) % BITMAP_PAGESIZE;
    char* tmp = reinterpret_cast<char*>(page_head_);
    tmp[page_index - 1] = tmp[page_index - 1] | (1 << page_offset);
    bit_cur_size_++;
    return ContainerRet::SUCCESS;
}

ContainerRet Bitmap::Unset(size_t index)
{
    if (!page_head_) {
        return ContainerRet::EINIT;
    }
    if (index == 0 || index > bit_max_size_) {
        return ContainerRet::BM_EOUTBOUND;
    }
    size_t page_index = (index + BITMAP_PAGESIZE - 1) / BITMAP_PAGESIZE;
    size_t page_offset = (index - 1 ) % BITMAP_PAGESIZE;
    char* tmp = reinterpret_cast<char*>(page_head_);
    tmp[page_index - 1] = tmp[page_index - 1] &~ (1 << page_offset);
    bit_cur_size_--;
    return ContainerRet::SUCCESS;
}

void Bitmap::Print1()
{
    size_t page_index = 0;
    size_t page_offset = 0;
    size_t tmp_max_bit = bit_max_size_;
    char* tmp = reinterpret_cast<char*>(page_head_);
    for (page_index = 0; page_index < page_num_; page_index++) {
        for (page_offset = 0; page_offset < BITMAP_PAGESIZE; page_offset++, tmp_max_bit--) {
            if (tmp_max_bit == 0) {
                break;
            }
            if ((tmp[page_index] & (1 << page_offset)) != 0) {
                printf("%lu ", (page_index * BITMAP_PAGESIZE + page_offset + 1));
            }
        }
        if (tmp_max_bit == 0) {
            break;
        }
    }
    printf("\n");
}

void Bitmap::Print0()
{
    size_t page_index = 0;
    size_t page_offset = 0;
    size_t tmp_max_bit = bit_max_size_;
    char* tmp = reinterpret_cast<char*>(page_head_);
    for (page_index = 0; page_index < page_num_; page_index++) {
        for (page_offset = 0; page_offset < BITMAP_PAGESIZE; page_offset++, tmp_max_bit--) {
            if (tmp_max_bit == 0) {
                break;
            }
            if ((tmp[page_index] & (1 << page_offset)) == 0) {
                printf("%lu ", (page_index * BITMAP_PAGESIZE + page_offset + 1));
            }
        }
        if (tmp_max_bit == 0) {
            break;
        }
    }
    printf("\n");
}

size_t Bitmap::Find1()
{
    size_t page_index = 0;
    size_t page_offset = 0;
    size_t tmp_max_bit = bit_max_size_;
    char* tmp = reinterpret_cast<char*>(page_head_);
    for (page_index = 0; page_index < page_num_; page_index++) {
        for (page_offset = 0; page_offset < BITMAP_PAGESIZE; page_offset++, tmp_max_bit--) {
            if (tmp_max_bit == 0) {
                break;
            }
            if ((tmp[page_index] & (1 << page_offset)) != 0) {
                return (page_index * BITMAP_PAGESIZE + page_offset + 1) ;
            }
        }
        if (tmp_max_bit == 0) {
            break;
        }
    }
    return 0;
}

size_t Bitmap::Find0()
{
    size_t page_index = 0;
    size_t page_offset = 0;
    size_t tmp_max_bit = bit_max_size_;
    char* tmp = reinterpret_cast<char*>(page_head_);
    for (page_index = 0; page_index < page_num_; page_index++) {
        for (page_offset = 0; page_offset < BITMAP_PAGESIZE; page_offset++, tmp_max_bit--) {
            if (tmp_max_bit == 0) {
                break;
            }
            if ((tmp[page_index] & (1 << page_offset)) == 0) {
                return (page_index * BITMAP_PAGESIZE + page_offset + 1) ;
            }
        }
        if (tmp_max_bit == 0) {
            break;
        }
    }
    return 0;
}

bool Bitmap::IsFull()
{
    if (Find0() == 0) {
        return true;
    } else {
        return false;
    }
}

size_t Bitmap::GetBitmapSize(size_t bit_size)
{
    return (bit_size + (BITMAP_PAGESIZE-1)) & ~(BITMAP_PAGESIZE-1);
}

void Bitmap::Print()
{
    std::string str;
    size_t page_index = 0;
    size_t page_offset = 0;
    size_t tmp_max_bit = bit_max_size_;
    char* tmp = reinterpret_cast<char*>(page_head_);
    for (page_index = 0; page_index < page_num_; page_index++) {
        for (page_offset = 0; page_offset < BITMAP_PAGESIZE; page_offset++, tmp_max_bit--) {
            if (tmp_max_bit == 0) {
                break;
            }
            if ((tmp[page_index] & (1 << page_offset)) != 0) {
                str += "1";
            } else {
                str += "0";
            }
        }
        if (tmp_max_bit == 0) {
            break;
        }
    }
    printf("%s\n", std::string(str.rbegin(), str.rend()).c_str());
}

}
