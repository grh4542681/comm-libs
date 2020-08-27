#ifndef __CONTAINER_BITMAP_H__
#define __CONTAINER_BITMAP_H__

#include "object.h"
#include "container_return.h"

#define BITMAP_PAGESIZE (8)

namespace infra::container {

class Bitmap : virtual public infra::base::Object {
public:
    Bitmap() { }
    Bitmap(size_t bit_size) : bit_max_size_(bit_size) {
        page_num_ = (bit_size + (BITMAP_PAGESIZE-1)) / BITMAP_PAGESIZE;
        page_head_ = new char[(page_num_ * BITMAP_PAGESIZE)];
        if (page_head_) {
            free_flag_ = true;
        }
    }
    Bitmap(const Bitmap& other) {
        page_num_ = other.page_num_;
        if (other.page_head_) {
            page_head_ = new char[(page_num_ * BITMAP_PAGESIZE)];
        }
        bit_max_size_= other.bit_max_size_;
        bit_cur_size_ = other.bit_cur_size_;
    }
    ~Bitmap() {
        if (page_head_) {
            delete page_head_;
        }
    }

    size_t GetMaxBitsize() {
        return bit_max_size_;
    }
    size_t GetCurBitsize() {
        return bit_cur_size_;
    }

    ContainerRet Set(size_t index) {
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
    ContainerRet Unset(size_t index) {
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
    size_t Find0() {
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
    size_t Find1() {
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
    bool IsFull() {
        if (Find0() == 0) {
            return true;
        } else {
            return false;
        }
    }

    void Print1() {
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
    void Print0() {
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
    void Print() {
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

    static size_t GetBitmapSize(size_t bit_size) {
        return (bit_size + (BITMAP_PAGESIZE-1)) & ~(BITMAP_PAGESIZE-1);
    }
private:
    mempool::Mempool* mempool_ = { mempool::Mempool::getInstance() };
    void* page_head_ = { NULL };
    size_t page_num_ = { 0 };
    size_t bit_max_size_ = { 0 };
    size_t bit_cur_size_ = { 0 };
};

}

#endif
