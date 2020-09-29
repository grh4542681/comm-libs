#ifndef __CONTAINER_BITMAP_H__
#define __CONTAINER_BITMAP_H__

#include "object.h"
#include "container_return.h"

#define BITMAP_PAGESIZE (sizeof(char) * 8)

namespace infra::container {

class Bitmap : virtual public infra::base::Object {
public:
    Bitmap() { }
    Bitmap(size_t bit_size) : bit_max_size_(bit_size) {
        page_num_ = (bit_size + (BITMAP_PAGESIZE-1)) / BITMAP_PAGESIZE;
        page_head_ = malloc(page_num_ * sizeof(char));
    }
    Bitmap(const Bitmap& other) {
        page_num_ = other.page_num_;
        if (page_head_) {
            free(page_head_);
            page_head_ = NULL;
        }
        if (other.page_head_) {
            page_head_ = malloc(page_num_ * sizeof(char));
        }
        bit_max_size_= other.bit_max_size_;
        bit_cur_size_ = other.bit_cur_size_;
        memcpy(page_head_, other.page_head_, page_num_ * sizeof(char));
    }
    ~Bitmap() {
        if (page_head_) {
            free(page_head_);
        }
    }

    const Bitmap& operator=(const Bitmap& other) {
        page_num_ = other.page_num_;
        if (page_head_) {
            free(page_head_);
            page_head_ = NULL;
        }
        if (other.page_head_) {
            page_head_ = malloc(page_num_ * sizeof(char));
        }
        bit_max_size_= other.bit_max_size_;
        bit_cur_size_ = other.bit_cur_size_;
        memcpy(page_head_, other.page_head_, page_num_ * sizeof(char));
        return *this;
    }

    size_t GetMaxBitsize() {
        return bit_max_size_;
    }
    size_t GetCurBitsize() {
        return bit_cur_size_;
    }

    Return Set(size_t index) {
        if (!page_head_) {
            return Return::CONTAINER_BITMAP_INIT;
        }
        if (index == 0 || index > bit_max_size_) {
            return Return::CONTAINER_BITMAP_EOUTBOUND;
        }
        size_t page_index = (index + BITMAP_PAGESIZE - 1) / BITMAP_PAGESIZE;
        size_t page_offset = (index - 1 ) % BITMAP_PAGESIZE;
        char* tmp = reinterpret_cast<char*>(page_head_);
        tmp[page_index - 1] = tmp[page_index - 1] | (1 << page_offset);
        bit_cur_size_++;
        return Return::SUCCESS;
    }
    Return Set(Bitmap&& bitmap) {
        if (!page_head_) {
            return Return::CONTAINER_BITMAP_INIT;
        }
        if (bitmap.GetMaxBitsize() > GetMaxBitsize()) {
            return Return::CONTAINER_BITMAP_EOUTBOUND;
        }
        size_t index = 0;
        while((index = bitmap.Find1() != 0)) {
            if (!Isset(index)) {
                Set(index);
                bit_cur_size_++;
            }
        }
        return Return::SUCCESS;
    }

    Return Unset(size_t index) {
        if (!page_head_) {
            return Return::CONTAINER_BITMAP_INIT;
        }
        if (index == 0 || index > bit_max_size_) {
            return Return::CONTAINER_BITMAP_EOUTBOUND;
        }
        size_t page_index = (index + BITMAP_PAGESIZE - 1) / BITMAP_PAGESIZE;
        size_t page_offset = (index - 1 ) % BITMAP_PAGESIZE;
        char* tmp = reinterpret_cast<char*>(page_head_);
        tmp[page_index - 1] = tmp[page_index - 1] &~ (1 << page_offset);
        bit_cur_size_--;
        return Return::SUCCESS;
    }
    Return Unset(Bitmap&& bitmap) {
        if (!page_head_) {
            return Return::CONTAINER_BITMAP_INIT;
        }
        if (bitmap.GetMaxBitsize() > GetMaxBitsize()) {
            return Return::CONTAINER_BITMAP_EOUTBOUND;
        }
        size_t index = 0;
        while((index = bitmap.Find1() != 0)) {
            if (Isset(index)) {
                Unset(index);
                bit_cur_size_--;
            }
        }
        return Return::SUCCESS;
    }

    bool Isset(size_t index) {
        if (!page_head_) {
            return false;
        }
        if (index == 0 || index > bit_max_size_) {
            return false;
        }
        size_t page_index = (index + BITMAP_PAGESIZE - 1) / BITMAP_PAGESIZE;
        size_t page_offset = (index - 1 ) % BITMAP_PAGESIZE;
        char* tmp = reinterpret_cast<char*>(page_head_);
        if (tmp[page_index - 1] & (1 << page_offset))
            return true;
        else
            return false;
    }
    bool Isset(Bitmap&& bitmap) {
        if (!page_head_) {
            return false;
        }
        if (bitmap.GetMaxBitsize() > GetMaxBitsize()) {
            return false;
        }
        size_t index = 0;
        while((index = bitmap.Find1() != 0)) {
            if (!Isset(index)) {
                return false;
            }
        }
        return true;
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
    bool IsFull() {
        if (Find0() == 0) {
            return true;
        } else {
            return false;
        }
    }

    std::string Print1() {
        std::string bitstr;
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
                    bitstr += std::to_string(page_index * BITMAP_PAGESIZE + page_offset + 1);
                    bitstr += " ";
                }
            }
            if (tmp_max_bit == 0) {
                break;
            }
        }
        return bitstr;
    }
    std::string Print0() {
        std::string bitstr;
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
                    bitstr += std::to_string(page_index * BITMAP_PAGESIZE + page_offset + 1);
                    bitstr += " ";
                }
            }
            if (tmp_max_bit == 0) {
                break;
            }
        }
        return bitstr;
    }
    std::string Print() {
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
        return str;
    }
private:
    void* page_head_ = { NULL };
    size_t page_num_ = { 0 };
    size_t bit_max_size_ = { 0 };
    size_t bit_cur_size_ = { 0 };
};

}

#endif
