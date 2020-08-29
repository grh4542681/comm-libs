/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-07-01 10:35
 * @file     : shm_list.h
 * @brief    : A list on share memory.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __SHM_LIST_H__
#define __SHM_LIST_H__

#include <string>
#include <typeinfo>
#include <type_traits>
#include <iterator>

#include "bitmap.h"
#include "timer_time.h"

#include "shm_sysv.h"
#include "shm_posix.h"

namespace ipc::shm {

/**
* @brief - A share memory list.
*
* @tparam [T] - Element type.
*/
template < typename S, typename T >
class ShmList {
public:
    typedef struct _ShmListNode {
        size_t index;
        size_t prev;
        size_t next;
    } ShmListNode;

    typedef struct _ShmListInfo {
        char object_name_[32 + 1];
        size_t object_size_;
        size_t object_max_num_;
        size_t object_cur_num_;
        size_t object_bitmap_offset_;
        size_t object_data_area_offset_;
        size_t object_head_index_;
        size_t object_tail_index_;

        bool rw_lock_enable_;
        bool mutex_lock_enable_;

        timer::Time create_time_;
        timer::Time last_access_time_;
    } ShmListInfo;

    class ShmListHead {
    public:
        ShmListHead() {

        }

        ShmListHead(ShmListInfo* info) {
            info_ = info;
            object_bitmap_ = reinterpret_cast<char*>(info) + info->object_bitmap_offset_;
            object_data_area_ = reinterpret_cast<char*>(info) + info->object_data_area_offset_;
            if (info->object_head_index_) {
                object_head_ = reinterpret_cast<ShmListNode*>(reinterpret_cast<char*>(object_data_area_) +
                                ((info->object_head_index_-1) * (sizeof(T) + sizeof(ShmListNode))) + sizeof(T));
            } else {
                object_head_ = nullptr;
            }
            if (info->object_tail_index_) {
                object_tail_ = reinterpret_cast<ShmListNode*>(reinterpret_cast<char*>(object_data_area_) +
                                ((info->object_tail_index_-1) * (sizeof(T) + sizeof(ShmListNode))) + sizeof(T));
            }
        }

        ~ShmListHead() {

        }

        ShmListNode* Next(ShmListNode* node) {
            if (node->next) {
                return reinterpret_cast<ShmListNode*>(reinterpret_cast<char*>(object_data_area_) + ((node->next -1) * (sizeof(T) + sizeof(ShmListNode))) + sizeof(T));
            }
            return NULL;
        }

        ShmListNode* Prev(ShmListNode* node) {
            if (node->prev) {
                return reinterpret_cast<ShmListNode*>(reinterpret_cast<char*>(object_data_area_) + ((node->prev -1) * (sizeof(T) + sizeof(ShmListNode))) + sizeof(T));
            }
            return NULL;
        }

    public:
        ShmListInfo* info_ = NULL;
        void* object_bitmap_ = NULL;
        void* object_data_area_ = NULL;
        ShmListNode* object_head_ = NULL;
        ShmListNode* object_tail_ = NULL;
    };

    class iterator : public std::iterator<std::input_iterator_tag, ShmListNode> {
    public:
        friend class ShmList;
        iterator(ShmListHead* head, ShmListNode* node = NULL) : head_(head),ptr(node) { }

        iterator(const iterator& other) : head_(other.head_), ptr(other.ptr) { } 
        ~iterator() { }

        T& operator*() { return *(reinterpret_cast<T*>(reinterpret_cast<char*>(ptr) - sizeof(T))); }
        bool operator!=(const iterator& other) const { return (ptr != other.ptr); }
        bool operator==(const iterator& other) const { return (ptr == other.ptr); }
        const iterator& operator--() {
            ptr = head_->Prev(ptr);
            return *this;
        }   
        const iterator operator--(int) {
            iterator tmp = *this;
            ptr = head_->Prev(ptr);
            return tmp;
        }   
        const iterator& operator++() {
            ptr = head_->Next(ptr);
            return *this;
        }   
        const iterator operator++(int) {
            iterator tmp = *this;
            ptr = head_->Next(ptr);
            return tmp;
        }
        ShmListHead* head_ = NULL;
        ShmListNode* ptr = NULL;
    };
public:
    ShmList(std::string path) : shm_(path) {
        static_assert(std::is_base_of<Shm, S>::value, "Must be a subclass that inherits from Shm.");
    }
    ~ShmList() {
        Close();
    }

    iterator begin () { return iterator(&shm_head_, shm_head_.object_head_); }
    iterator end () const { return iterator(NULL, NULL); }

    Shm& GetShm() {
        return shm_;
    }

    std::string GetObjectName() {
        return shm_head_.info_ ? shm_head_.info_->object_name_ : "";
    }

    size_t GetObjectSize() {
        return shm_head_.info_ ? shm_head_.info_->object_size_ : 0;
    }

    size_t GetObjectMaxNumber() {
        return shm_head_.info_ ? shm_head_.info_->object_max_num_ : 0;
    }

    size_t GetObjectCurNumber() {
        return shm_head_.info_ ? shm_head_.info_->object_cur_num_ : 0;
    }

    IpcRet Create(size_t obj_num, mode_t mode) {
        size_t shm_size = sizeof(ShmListInfo) + container::Bitmap::GetBitmapSize(obj_num) + obj_num * (sizeof(T) + sizeof(ShmListNode));
        IpcRet ret = shm_.Create(shm_size, mode);
        if (ret != IpcRet::SUCCESS) {
            return ret;
        }
        ret = shm_.Open(IpcMode::READ_WRITE);
        if (ret == IpcRet::SUCCESS) {
            ShmListInfo* p_shm_info_ = reinterpret_cast<ShmListInfo*>(shm_.GetHeadPtr());

            memcpy(p_shm_info_->object_name_, typeid(T).name(), sizeof(p_shm_info_->object_name_));
            p_shm_info_->object_size_ = sizeof(T);
            p_shm_info_->object_max_num_ = obj_num;
            p_shm_info_->object_cur_num_ = 0;
            p_shm_info_->object_bitmap_offset_ = sizeof(ShmListInfo);
            p_shm_info_->object_data_area_offset_ = sizeof(ShmListInfo) + container::Bitmap::GetBitmapSize(obj_num);
            p_shm_info_->object_head_index_ = 0;
            p_shm_info_->object_tail_index_ = 0;

            p_shm_info_->create_time_ = timer::Time::Now();
            p_shm_info_->last_access_time_ = timer::Time::Now();
        }
        ret = shm_.Close();
        if (ret == IpcRet::SUCCESS) {
            shm_head_ = ShmListHead();
        }

        return ret;
    }
    IpcRet Destroy() {
        IpcRet ret = shm_.Destroy();
        if (ret == IpcRet::SUCCESS) {
            shm_head_ = ShmListHead();
        }
        return ret;
    }
    IpcRet Open(IpcMode mode) {
        IpcRet ret = shm_.Open(mode);
        if (ret == IpcRet::SUCCESS) {
            ShmListInfo* p_shm_info_ = reinterpret_cast<ShmListInfo*>(shm_.GetHeadPtr());
            p_shm_info_->last_access_time_ = timer::Time::Now();
            shm_head_ = ShmListHead(p_shm_info_);
        }
        return ret;
    }
    IpcRet Close() {
        IpcRet ret = shm_.Close();
        if (ret == IpcRet::SUCCESS) {
            shm_head_ = ShmListHead();
        }
        return ret;
    }

    template < typename ... Args > T* PushBefore(iterator& node, Args&& ... args) {
        return _push_before(node.ptr, std::forward<Args>(args)...);
    }
    template < typename ... Args > T* PushAfter(iterator& node, Args&& ... args) {
        return _push_after(node.ptr, std::forward<Args>(args)...);
    }
    template < typename ... Args > T* PushHead(Args&& ... args) {
        return _push_before(shm_head_.object_head_, std::forward<Args>(args)...);
    }
    template < typename ... Args > T* PushTail(Args&& ... args) {
        return _push_after(shm_head_.object_tail_, std::forward<Args>(args)...);
    }

    void Pop(iterator& node) {
        _pop(node.ptr);
    }
    void PopHead() {
        _pop(shm_head_.object_head_);
    }
    void PopTail() {
        _pop(shm_head_.object_tail_);
    }

    IpcRet Format();

private:
    template < typename ... Args > T* _push_before(ShmListNode* node, Args&& ... args) {
        void* parea = NULL;
        T* pdata = NULL;
        ShmListNode* pnode = NULL;
        size_t bit_index = 0;

        if (!shm_head_.info_) {
            ret_ = IpcRet::EINIT;
            return NULL;
        }
        if (shm_head_.info_->object_max_num_ == shm_head_.info_->object_cur_num_) {
            ret_ = IpcRet::SL_ENOSPACE;
            return NULL;
        }

        container::Bitmap bm(shm_head_.info_->object_max_num_, shm_head_.object_bitmap_);
        bit_index = bm.Find0();
        if (bit_index == 0) {
            ret_ = IpcRet::SL_EBITMAP;
            return NULL;
        }
        parea = reinterpret_cast<void*>(reinterpret_cast<char*>(shm_head_.object_data_area_) + ((bit_index -1 ) * (sizeof(T) + sizeof(ShmListNode))));
        pdata  = mempool::Mempool::Construct<T>(parea, std::forward<Args>(args)...);
        if (!pdata) {
            ret_ = IpcRet::ECONSTRUCT;
            return NULL;
        }

        if (bm.Set(bit_index) != container::ContainerRet::SUCCESS) {
            ret_ = IpcRet::SL_EBITMAP;
            return NULL;
        }

        pnode = reinterpret_cast<ShmListNode*>(reinterpret_cast<char*>(parea) + sizeof(T));
        pnode->index = bit_index;

        if (!node) {
            if (!shm_head_.object_head_ && !shm_head_.object_tail_) {
                shm_head_.object_head_ = shm_head_.object_tail_ = pnode;
                shm_head_.info_->object_head_index_ = shm_head_.info_->object_tail_index_ = pnode->index;
                pnode->next = pnode->prev = 0;
            } else {
                shm_head_.object_head_->prev = pnode->index;
                pnode->next = shm_head_.object_head_->index;
                pnode->prev = 0;
                shm_head_.object_head_ = pnode;
                shm_head_.info_->object_head_index_ = pnode->index;
            }
        } else {
            pnode->next = node->index;
            if (shm_head_.Prev(node)) {
                pnode->prev = shm_head_.Prev(node)->index;
                shm_head_.Prev(node)->next = pnode->index;
            } else {
                pnode->prev = 0;
            }
            node->prev = pnode->index;
            if (node == shm_head_.object_head_) {
                shm_head_.object_head_ = pnode;
                shm_head_.info_->object_head_index_ = pnode->index;
            }
        }

        shm_head_.info_->object_cur_num_++;

        return pdata;
    }
    template < typename ... Args > T* _push_after(ShmListNode* node, Args&& ... args) {
        void* parea = NULL;
        T* pdata = NULL;
        ShmListNode* pnode = NULL;
        size_t bit_index = 0;

        if (!shm_head_.info_) {
            ret_ = IpcRet::EINIT;
            return NULL;
        }
        if (shm_head_.info_->object_max_num_ == shm_head_.info_->object_cur_num_) {
            ret_ = IpcRet::SL_ENOSPACE;
            return NULL;
        }

        container::Bitmap bm(shm_head_.info_->object_max_num_, shm_head_.object_bitmap_);
        bit_index = bm.Find0();
        if (bit_index == 0) {
            ret_ = IpcRet::SL_EBITMAP;
            return NULL;
        }
        parea = reinterpret_cast<void*>(reinterpret_cast<char*>(shm_head_.object_data_area_) + ((bit_index -1 ) * (sizeof(T) + sizeof(ShmListNode))));
        pdata  = mempool::Mempool::Construct<T>(parea, std::forward<Args>(args)...);
        if (!pdata) {
            ret_ = IpcRet::ECONSTRUCT;
            return NULL;
        }

        if (bm.Set(bit_index) != container::ContainerRet::SUCCESS) {
            ret_ = IpcRet::SL_EBITMAP;
            return NULL;
        }

        pnode = reinterpret_cast<ShmListNode*>(reinterpret_cast<char*>(parea) + sizeof(T));
        pnode->index = bit_index;

        if (!node) {
            if (!shm_head_.object_head_ && !shm_head_.object_tail_) {
                shm_head_.object_head_ = shm_head_.object_tail_ = pnode;
                shm_head_.info_->object_head_index_ = shm_head_.info_->object_tail_index_ = pnode->index;
                pnode->next = pnode->prev = 0;
            } else {
                shm_head_.object_tail_->next = pnode->index;
                pnode->prev = shm_head_.object_tail_->index;
                pnode->next = 0;
                shm_head_.object_tail_ = pnode;
                shm_head_.info_->object_tail_index_ = pnode->index;
            }
        } else {
            pnode->prev = node->index;
            if (shm_head_.Next(node)) {
                pnode->next = shm_head_.Next(node)->index;
                shm_head_.Next(node)->prev = pnode->index;
            } else {
                pnode->next = 0;
            }
            node->next = pnode->index;
            if (node == shm_head_.object_tail_) {
                shm_head_.object_tail_ = pnode;
                shm_head_.info_->object_tail_index_ = pnode->index;
            }
        }

        shm_head_.info_->object_cur_num_++;

        return pdata;
    }

    void _pop(ShmListNode* node) {
        if (!shm_head_.info_) {
            ret_ = IpcRet::EINIT;
            return;
        }
        if (!node) {
            ret_ = IpcRet::EBADARGS;
            return;
        }

        size_t bit_index = node->index;
        container::Bitmap bm(shm_head_.info_->object_max_num_, shm_head_.object_bitmap_);
        bm.Unset(bit_index);

        T* pdata = reinterpret_cast<T*>(reinterpret_cast<char*>(node) - sizeof(T));
        if (node->next && node->prev) {
            shm_head_.Prev(node)->next = node->next;
            shm_head_.Next(node)->prev = node->prev;
        } else if (node->next) {
            shm_head_.Next(node)->prev = node->prev;
            shm_head_.object_head_ = shm_head_.Next(node);
            shm_head_.info_->object_head_index_ = shm_head_.Next(node)->index;
        } else if (node->prev) {
            shm_head_.Prev(node)->next = node->next;
            shm_head_.object_tail_ = shm_head_.Prev(node);
            shm_head_.info_->object_tail_index_ = shm_head_.Prev(node)->index;
        } else {
            shm_head_.object_head_ = shm_head_.object_tail_ = NULL;
            shm_head_.info_->object_head_index_ = shm_head_.info_->object_tail_index_ = 0;
        }
        mempool::Mempool::Destruct<T>(pdata);

        shm_head_.info_->object_cur_num_--;
    }

private:
    ShmListHead shm_head_;
    S shm_;
    IpcRet ret_;
};

}

#endif
