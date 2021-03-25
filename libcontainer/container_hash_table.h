/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-01 03:41
 * @file     : hash_table.h
 * @brief    : 
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "mempool.h"
#include "mutex/thread_rw_lock.h"

namespace infra::container {


typedef unsigned long (*StringHash)(const char*);

template < typename T, typename F = StringHash >
class HashTable {
public:
    //hash function
    static unsigned long BKDRHash(const char* str) {
        unsigned long hash = 0;
        if (!str) {
            return 0;
        }
        while (*str) {
            hash = hash * 31 + (*str++);
        }
        return (hash);
    }

public:
    /**
    * @brief - Hash table return value
    */
    enum class Ret : int {
        SUCCESS = 0,    ///< sucess.
        ERROR = -999,   ///< error.
        EHASHINDEX,     ///< bad hash value.
        ERLOCK,         ///< read lock error.
        EWLOCK,         ///< write lock error.
        EUNLOCK,        ///< release lock error.
    };
    
    /**
    * @brief HashTable - Default Construct function.
    *
    * @param [size] - Max hash table slots size.
    */
    HashTable(unsigned long size) : size_(size) {
        freesize_ = size_;
        count_ = 0;
        thread_safe_flag_ = false;
        mp = mempool::Mempool::getInstance();
        memblock = mp->Malloc(sizeof(T)*size);
        memset(memblock, 0x00, sizeof(T)*size);
        hashcode_ = BKDRHash;
    }

    /**
    * @brief HashTable - Construct function.
    *
    * @param [size] - Max hash table slots size.
    * @param [hashcode] - Hash function.
    */
    HashTable(unsigned long size, F hashcode) : size_(size), hashcode_(hashcode) {
        freesize_ = size_;
        count_ = 0;
        thread_safe_flag_ = false;
        mp = mempool::Mempool::getInstance();
        memblock = mp->Malloc(sizeof(T)*size_);
        memset(memblock, 0x00, sizeof(T)*size_);
    }

    /**
    * @brief HashTable - Copy construct function.
    *
    * @param [other] - Other instance.
    */
    HashTable(HashTable& other) {
        size_ = other.size_;
        hashcode_ = other.hashcode_;
        thread_safe_flag_ = other.thread_safe_flag_;

        freesize_ = size_;
        count_ = 0;
        mp = mempool::Mempool::getInstance();
        memblock = mp->Malloc(sizeof(T)*size_);
        memset(memblock, 0x00, sizeof(T)*size_);
        T* p = NULL;
        unsigned long loop = 0;
        for (p = (T*)memblock, loop = size_; loop > 0; --loop,++p) {
            insert(*p, size_ - loop);    
        }
    }

    /**
    * @brief ~HashTable - Destruct function.
    */
    ~HashTable() {
        empty();
        mp->Free(memblock);
    }

    /**
    * @brief size - Get current hash table max slots size.
    *
    * @returns  Hash table max size.
    */
    unsigned long size() { return size_; }
    /**
    * @brief count - Get current hast table element count.
    *
    * @returns  Element count.
    */
    unsigned long count() { return count_; }
    /**
    * @brief available - Get current hash table available slots.
    *
    * @returns  Available slots size.
    */
    unsigned long available() { return freesize_; }
    /**
    * @brief setThreadSafe - Enable thread safety.
    *
    * @param [flag] - True or false.
    */
    void setThreadSafe(bool flag) { thread_safe_flag_ = flag; }
    /**
    * @brief getThreadSafe - Get thread safety flag.
    *
    * @returns  True or false.
    */
    bool getThreadSafe() { return thread_safe_flag_; }
    /**
    * @brief setNonBlock - Set non-blocking mode.
    *
    * @param [flag] - True or false.
    */
    void setNonBlock(bool flag) { rwlock_.setNonBlock(flag); }
    /**
    * @brief getNonBlock - Get current blocking mode.
    *
    * @returns  True or false.
    */
    bool getNonBlock() { return rwlock_.getNonBlock(); }
    /**
    * @brief getLastRet - Get last return value.
    *
    * @returns  Ret.
    */
    Ret getLastRet(){ return last_return_; }

    /**
    * @brief hashcode - Calculate the hash value.
    *
    * @tparam [Args] - Template arguments of hash function.
    * @param [args] - Hash function arguments.
    *
    * @returns  Hash value.
    */
    template<typename ... Args>
    unsigned long hashcode(Args&& ... args) {
        return (hashcode_(std::forward<Args>(args)...) % size_);
    }

    /**
    * @brief insert - Insert an element into hash table.
    *
    * @tparam [Args] - Template arguments of element constructor.
    * @param [args] - Element constructor arguments.
    * @param [hash] - Hash value.
    *
    * @returns  Element pointer. If return NULL, you can get last return value
    *           by getLastRet().
    */
    template<typename ... Args>
    T* insert(Args&& ... args, unsigned long hash) {
        if (hash > size_) {
            _setret(Ret::EHASHINDEX);
            return NULL;
        }
        void* offset = reinterpret_cast<char*>(memblock) + hash * sizeof(T);
        T* pnode = NULL;
        if (*reinterpret_cast<char*>(offset)) {
            mempool::Mempool::Destruct<T>(reinterpret_cast<T*>(offset));
            pnode = mempool::Mempool::Construct<T>(offset, std::forward<Args>(args)...);
        } else {
            pnode = mempool::Mempool::Construct<T>(offset, std::forward<Args>(args)...);
            ++count_;
            --freesize_;
        }
        return pnode;
    }

    /**
    * @brief insert - Insert an element into hash table.
    *
    * @tparam [Args] - Template arguments of hash function.
    * @param [data] - Element reference.
    * @param [args] - Arguments for hash function.
    *
    * @returns  Element pointer. If return NULL, you can get last return value
    *           by getLastRet().
    */
    template<typename ... Args>
    T* insert(T& data, Args&& ... args) {
        return insert(std::move(data), std::forward<Args>(args)...);
    }
    template<typename ... Args>
    T* insert(T&& data, Args&& ... args) {
        unsigned long hash = hashcode_(std::forward<Args>(args)...) % size_;
        if (hash > size_) {
            _setret(Ret::EHASHINDEX);
            return NULL;
        }
        void* offset = reinterpret_cast<char*>(memblock) + hash * sizeof(T);
        T* pnode = NULL;
        if (*reinterpret_cast<char*>(offset)) {
            mempool::Mempool::Destruct<T>(reinterpret_cast<T*>(offset));
            pnode = mempool::Mempool::Construct<T>(offset, data);
        } else {
            pnode = mempool::Mempool::Construct<T>(offset, data);
            ++count_;
            --freesize_;
        }
        return pnode;
    }

    /**
    * @brief insert - Insert an element into hash table.
    *
    * @param [data] - Element referene.
    * @param [hash] - Hash value.
    *
    * @returns  Element pointer. If return NULL, you can get last return value
    *           by getLastRet().
    */
    T* insert(T& data, unsigned long hash) {
        return insert(std::move(data), hash);
    }
    T* insert(T&& data, unsigned long hash) {
        if (hash > size_) {
            _setret(Ret::EHASHINDEX);
            return NULL;
        }
        void* offset = reinterpret_cast<char*>(memblock) + hash * sizeof(T);
        T* pnode = NULL;
        if (*reinterpret_cast<char*>(offset)) {
            mempool::Mempool::Destruct<T>(reinterpret_cast<T*>(offset));
            pnode = mempool::Mempool::Construct<T>(offset, data);
        } else {
            pnode = mempool::Mempool::Construct<T>(offset, data);
            ++count_;
            --freesize_;
        }
        return pnode;
    }

    /**
    * @brief remove - Remove an element from hash table.
    *
    * @tparam [Args] - Template arguments of hash function.
    * @param [args] - Arguments for hash function.
    */
    template<typename ... Args>
    void remove(Args&& ... args) {
        unsigned long hash = hashcode_(std::forward<Args>(args)...) % size_;
        remove(hash);
    }
    void remove(unsigned long hash) {
        if (hash > size_) {
            _setret(Ret::EHASHINDEX);
            return;
        }
        void* offset = reinterpret_cast<char*>(memblock) + hash * sizeof(T);
        if (*reinterpret_cast<char*>(offset)) {
            mempool::Mempool::Destruct<T>(reinterpret_cast<T*>(offset));
            memset(offset, 0x00, sizeof(T));
            --count_;
            ++freesize_;
        }
    }

    /**
    * @brief find - Find an element from hash table
    *
    * @tparam [Args] - Template arguments of hash function.
    * @param [args] - Arguments for hash function.
    *
    * @returns  Element pointer. If return NULL, you can get last return value
    *           by getLastRet().
    */
    template<typename ... Args>
    T* find(Args&& ... args) {
        unsigned long hash = hashcode_(std::forward<Args>(args)...) % size_;
        return find(hash);
    }
    T* find(unsigned long hash) {
        if (hash > size_) {
            _setret(Ret::EHASHINDEX);
            return NULL;
        }
        void* offset = reinterpret_cast<char*>(memblock) + hash * sizeof(T);
        return (*reinterpret_cast<char*>(offset)) ? reinterpret_cast<T*>(offset) : NULL;
    }

    /**
    * @brief empty - Clean all element in hash table
    */
    void empty() {
        for (unsigned long loop = 0; loop < size_; loop++) {
            void* offset = reinterpret_cast<char*>(memblock) + loop * sizeof(T);
            if (*reinterpret_cast<char*>(offset)) {
                mempool::Mempool::Destruct<T>(reinterpret_cast<T*>(offset));
                memset(offset, 0x00, sizeof(T));
                --count_;
                ++freesize_;
            }
        }
    }

    /**
    * @brief rlock - Get read lock.
    *
    * @param [overtime] - Over time.
    *
    * @returns  Ret.
    */
    Ret rlock(struct timespec* overtime) {
        if (!thread_safe_flag_) {
            return Ret::SUCCESS;
        } else {
            if (rwlock_.RLock(overtime) != thread::ThreadRet::SUCCESS) {
                return Ret::ERLOCK;
            } else {
                return Ret::SUCCESS;
            }
        }
    }
    /**
    * @brief wlock - Get write lock.
    *
    * @param [overtime] - Over time.
    *
    * @returns  Ret.
    */
    Ret wlock(struct timespec* overtime) {
        if (!thread_safe_flag_) {
            return Ret::SUCCESS;
        } else {
            if (rwlock_.WLock(overtime) != thread::ThreadRet::SUCCESS) {
                return Ret::EWLOCK;
            } else {
                return Ret::SUCCESS;
            }
        }
    }
    /**
    * @brief unlock - Release lock/
    *
    * @returns  Ret.
    */
    Ret unlock() {
        if (rwlock_.UnLock() != thread::ThreadRet::SUCCESS) {
            return Ret::EUNLOCK;
        } else {
            return Ret::SUCCESS;
        }
    }

private:
    unsigned long size_;        ///< hash table max slots.
    unsigned long freesize_;    ///< available slots size.
    unsigned long count_;       ///< current element count.
    bool thread_safe_flag_;     ///< thread safety flag.
    F hashcode_;                ///< hash function.
    mempool::Mempool* mp;           ///< mempool interface pointer.
    thread::mutex::ThreadRWLock rwlock_;   ///< read write thread lock instance.
    void* memblock;             ///< hash table memory.

    Ret last_return_;           ///< last return value.
    void _setret(Ret ret) { last_return_ = ret; }
};

} //namespace end

#endif
