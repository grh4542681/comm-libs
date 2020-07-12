/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-01 04:09
 * @file     : link_list.h
 * @brief    : 
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__

#include <type_traits>
#include <iterator>
#include "mempool.h"

namespace container {

/**
* @brief - Doubly linked list class.
*
* @tparam [T] - Element type.
*/
template <typename T>
class LinkList {
private:
    /**
    * @brief - List element node.
    */
    struct LinkNode {
        T* data_;
        struct LinkNode* prev;
        struct LinkNode* next;
    };
public:
    /**
    * @brief - List iterator.
    */
    class iterator : public std::iterator<std::input_iterator_tag, struct LinkNode> {
    public:
        friend class LinkList;
        iterator(struct LinkNode* node = NULL) : ptr(node) {}
        iterator(const iterator& other) : ptr(other.ptr) { }
        ~iterator() {}

        T& operator*() { return *(ptr->data_); }
        bool operator!=(const iterator& other) const { return (ptr != other.ptr); }
        bool operator==(const iterator& other) const { return (ptr == other.ptr); }
        const iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }
        const iterator operator--(int) {
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        const iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        const iterator operator++(int) {
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        void reset(T&& newdata) {
            ptr->reset(newdata);
        }
    private:
        struct LinkNode* ptr;
    };

public:
    struct LinkNode* head;  ///< list head element.
    struct LinkNode* tail;  ///< list tail element.

    /**
    * @brief LinkList - Default construct function.
    */
    LinkList() {
        head = NULL;
        tail = NULL;
        count_ = 0;
        mp = mempool::Mempool::getInstance();
    }
    /**
    * @brief LinkList - Copy construct function.
    *
    * @param [other] - Source instance.
    */
    LinkList(LinkList& other) {
        head = NULL;
        tail = NULL;
        count_ = 0;
        mp = mempool::Mempool::getInstance();
        for (auto it : other) {
            pushback(it);                   
        }
    }
    /**
    * @brief ~LinkList - Destruct function
    */
    ~LinkList() {
        clear();
    }

    LinkList& operator=(LinkList& other) {
        clear();
        for (auto it : other) {
            pushback(it);
        }
    }

    /**
    * @brief begin - Get head node iterator.
    *
    * @returns  Element iterator.
    */
    iterator begin () const { return iterator(head); }
    /**
    * @brief end - Tail iterator.(It should be NULL).
    *
    * @returns  Element iterator.
    */
    iterator end () const { return iterator(NULL); }
    /**
    * @brief count - Get current element count in list.
    *
    * @returns  Element count.
    */
    unsigned int count() { return count_; }

    /**
    * @brief pushback - Push an element at the head of list.
    *
    * @tparam [Args] - Template arguments of element constuctor.
    * @param [args] - Arguments of element constructor.
    */
    template<typename ... Args>
    void pushback(Args&& ... args) {
        void* ptr = mp->Malloc(sizeof(LinkNode) + sizeof(T));
        T* pdata = mempool::Mempool::Construct<T>(ptr, std::forward<Args>(args)...);
        struct LinkNode* pnode = reinterpret_cast<struct LinkNode*>(reinterpret_cast<char*>(ptr) + sizeof(T));
        pnode->data_ = pdata;
        _push_after(tail, pnode);
    }

    /**
    * @brief pushfront - Push an element at the end of list.
    *
    * @tparam [Args] - Template arguments of element constructor.
    * @param [args] - Arguments of element constructor.
    */
    template<typename ... Args>
    void pushfront(Args&& ... args) {
        void* ptr = mp->Malloc(sizeof(LinkNode) + sizeof(T));
        T* pdata = mempool::Mempool::Construct<T>(ptr, std::forward<Args>(args)...);
        struct LinkNode* pnode = reinterpret_cast<struct LinkNode*>(reinterpret_cast<char*>(ptr) + sizeof(T));
        pnode->data_ = pdata;
        _push_before(head, pnode);
    }

    /**
    * @brief pushbefore - Push an element before the iterator.
    *
    * @tparam [Args] - Template arguments of element constructor.
    * @param [target] - Element iterator.
    * @param [args] - Arguments of element constructor.
    */
    template<typename ... Args>
    void pushbefore(iterator& target, Args&& ... args) {
        void* ptr = mp->Malloc(sizeof(LinkNode) + sizeof(T));
        T* pdata = mempool::Mempool::Construct<T>(ptr, std::forward<Args>(args)...);
        struct LinkNode* pnode = reinterpret_cast<struct LinkNode*>(reinterpret_cast<char*>(ptr) + sizeof(T));
        pnode->data_ = pdata;
        _push_before(target.ptr, pnode);
    }

    /**
    * @brief pushafter - Push an element after the iterator.
    *
    * @tparam [Args] - Template arguments of element constructor.
    * @param [target] - Element iterator.
    * @param [args] - Arguments of element constructor.
    */
    template<typename ... Args>
    void pushafter(iterator& target, Args&& ... args) {
        void* ptr = mp->Malloc(sizeof(LinkNode) + sizeof(T));
        T* pdata = mempool::Mempool::Construct<T>(ptr, std::forward<Args>(args)...);
        struct LinkNode* pnode = reinterpret_cast<struct LinkNode*>(reinterpret_cast<char*>(ptr) + sizeof(T));
        pnode->data_ = pdata;
        _push_after(target.ptr, pnode);
    }

    /**
    * @brief pop - Pop(delete) an element.
    *
    * @param [target] - Element iterator.
    */
    void pop(iterator& target) {
        struct LinkNode* pnode = _pop(target.ptr);
        if (pnode) {
            mempool::Mempool::Destruct<T>(pnode->data_);
            mp->Free((void*)(pnode->data_));
        }
    }

    /**
    * @brief popfront - Pop(delete) list's first element.
    */
    void popfront() {
        struct LinkNode* pnode = _pop(head);
        if (pnode) {
            mempool::Mempool::Destruct<T>(pnode->data_);
            mp->Free((void*)(pnode->data_));
        }
    }

    /**
    * @brief popback - Pop(delete) list's last element.
    */
    void popback() {
        struct LinkNode* pnode = _pop(tail);
        if (pnode) {
            mempool::Mempool::Destruct<T>(pnode->data_);
            mp->Free((void*)(pnode->data_));
        }
    }

    /**
    * @brief clear - Pop(delete) all element.
    */
    void clear() {
        while (head) {
            popfront();
        }
        count_ = 0;
    }
private:
    unsigned int count_;    ///< current element counts.
    mempool::Mempool* mp;       ///< mempool interface pointer.

    void _push_before(struct LinkNode* currnode, struct LinkNode* newnode) {
        if (!newnode) {
            return;
        }
        if (!currnode) {
            head = tail = newnode;
            newnode->prev = NULL;
            newnode->next = NULL;
        } else {
            if (!(currnode->prev)) {
                newnode->prev = currnode->prev;
                newnode->next = currnode;
                currnode->prev = newnode;
                head = newnode;
            } else {
                currnode->prev->next = newnode;
                newnode->prev = currnode->prev;
                currnode->prev = newnode;
                newnode->next = currnode;
            }
        }
        ++count_;
    }

    void _push_after(struct LinkNode* currnode, struct LinkNode* newnode) {
        if (!newnode) {
            return;
        }
        if (!currnode) {
            head = tail = newnode;
            newnode->prev = NULL;
            newnode->next = NULL;
        } else {
            if (!(currnode->next)) {
                newnode->next = currnode->next;
                newnode->prev = currnode;
                currnode->next = newnode;
                tail = newnode;
            } else {
                currnode->next->prev = newnode;
                newnode->next = currnode->next;
                currnode->next = newnode;
                newnode->prev = currnode;
            }
        }
        ++count_;
    }

    struct LinkNode* _pop(struct LinkNode* node) {
        if (!node) {
            return NULL;
        }
        if (node->next && node->prev) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        } else if (node->next) {
            node->next->prev = node->prev;
            head = node->next;
        } else if (node->prev) {
            node->prev->next = node->next;
            tail = node->prev;
        } else {
            head = NULL;
            tail = NULL;
        }
        --count_;
        return node;
    }
};

} //namespace end

#endif
