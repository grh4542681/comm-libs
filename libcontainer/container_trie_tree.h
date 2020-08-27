/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-02-28 10:36
 * @file     : trie_tree.h
 * @brief    : Trie tree header file. It need libgmp.so for 
 *             alloc memory.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __TRIE_TREE_H__
#define __TRIE_TREE_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include "mempool.h"

#include "container_return.h"

namespace container{
/*
 * Dictionary supported characters (ASCII code)
 * 
 * Dec Hex   char |  Dec  Hex   char  |  Dec  Hex    char
 * 32 0x20  space |   68  0x44    D   |   104 0x68    h
 * 33 0x21    !   |   69  0x45    E   |   105 0x69    i
 * 34 0x22    "   |   70  0x46    F   |   106 0x6A    j
 * 35 0x23    #   |   71  0x47    G   |   107 0x6B    k
 * 36 0x24    $   |   72  0x48    H   |   108 0x6C    l
 * 37 0x25    %   |   73  0x49    I   |   109 0x6D    m
 * 38 0x26    &   |   74  0x4A    J   |   110 0x6E    n
 * 39 0x27    '   |   75  0x4B    K   |   111 0x6F    o
 * 40 0x28    (   |   76  0x4C    L   |   112 0x70    p
 * 41 0x29    )   |   77  0x4D    M   |   113 0x71    q
 * 42 0x2A    *   |   78  0x4E    N   |   114 0x72    r
 * 43 0x2B    +   |   79  0x4F    O   |   115 0x73    s
 * 44 0x2C    ,   |   80  0x50    P   |   116 0x74    t
 * 45 0x2D    -   |   81  0x51    Q   |   117 0x75    u
 * 46 0x2E    .   |   82  0x52    R   |   118 0x76    v
 * 47 0x2F    /   |   83  0x53    S   |   119 0x77    w
 * 48 0x30    0   |   84  0x54    T   |   120 0x78    x
 * 49 0x31    1   |   85  0x55    U   |   121 0x79    y
 * 50 0x32    2   |   86  0x56    V   |   122 0x7A    z
 * 51 0x33    3   |   87  0x57    W   |   123 0x7B    {
 * 52 0x34    4   |   88  0x58    X   |   124 0x7C    |
 * 53 0x35    5   |   89  0x59    Y   |   125 0x7D    }
 * 54 0x36    6   |   90  0x5A    Z   |   126 0x7E    ~
 * 55 0x37    7   |   91  0x5B    [
 * 56 0x38    8   |   92  0x5C    \
 * 57 0x39    9   |   93  0x5D    ]
 * 58 0x3A    :   |   94  0x5E    ^
 * 59 0x3B    ;   |   95  0x5F    _
 * 60 0x3C    <   |   96  0x60    `
 * 61 0x3D    =   |   97  0x61    a
 * 62 0x3E    >   |   98  0x62    b
 * 63 0x3F    ?   |   99  0x63    c
 * 64 0x40    @   |   100 0x64    d
 * 65 0x41    A   |   101 0x65    e
 * 66 0x42    B   |   102 0x66    f
 * 67 0x43    C   |   103 0x67    g
 */

#define TT_DEFAULT_MAX_KEY_LENGTH (128) ///< Default max tire tree key lengtn.
#define TT_DICT_SIZE (95)               ///< Tire tree dictionary size.
#define TT_DICT_INDEX(c) (c & (~0x20))  ///< Calculate the offset of a character.
#define TT_DICT_CHAR(c) (c | (0x20))    ///< Calculate characters by offset.

/**
* @brief - Tire Tree class.
*
* @tparam [T] - Element type.
*/
template < typename T >
class TrieTree {
public:
    /**
    * @brief - Data struct of tire tree elements.
    */
    struct TTNode {
        int level_;                             ///< current node level(length of the current key).
        unsigned int offset_;                   ///< offset in parent node word slots.
        T* data_;                               ///< element data.
        TTNode* parent_;                        ///< parent node pointer.
        struct TTNode* slots_[TT_DICT_SIZE];    ///< next word slots.
    };

public:
    /**
    * @brief TrieTree - Default construct function.
    */
    TrieTree() {
        count_ = 0;
        TT_max_key_length = TT_DEFAULT_MAX_KEY_LENGTH;
        mp_ = mempool::Mempool::getInstance();        
        root_ = reinterpret_cast<struct TTNode*>(mp_->Malloc(sizeof(struct TTNode)));
        _TTNode_init(root_);
    }
    /**
    * @brief TrieTree - Construct function.
    *
    * @param [max_len] - Key maximum length limit.
    */
    TrieTree(unsigned int max_len) {
        count_ = 0;
        TT_max_key_length = max_len;
        mp_ = mempool::Mempool::getInstance();        
        root_ = reinterpret_cast<struct TTNode*>(mp_->Malloc(sizeof(struct TTNode)));
        _TTNode_init(root_);
    }
    /**
    * @brief TrieTree - Copy construct function.
    *
    * @param [other] - Other instance.
    */
    TrieTree(TrieTree& other) {
        count_ = 0;
        TT_max_key_length = other.TT_max_key_length;
        mp_ = mempool::Mempool::getInstance();
        root_ = reinterpret_cast<struct TTNode*>(mp_->Malloc(sizeof(struct TTNode)));
        _TTNode_init(root_);
        _TTNode_insert_tree(other.getRoot());
    }
    /**
    * @brief ~TrieTree - Destruct function.
    */
    ~TrieTree() {
        _TTNode_delete_tree(root_);
    }

    /**
    * @brief getMaxKeyLen - Get the max key length in this TrieTree.
    *
    * @returns  Key maximum length limit.
    */
    unsigned int getMaxKeyLen() { return TT_max_key_length; }
    /**
    * @brief getCount -Get count of elements int this TrieTree now. 
    *
    * @returns  Element counts.
    */
    unsigned int getCount() { return count_; }

    TTNode* getRoot() { return root_; }

    /**
    * @brief getLastRet - Get last return value.
    *
    * @returns  ContainerRet.
    */
    ContainerRet getLastRet() { return last_return_; }

    /**
    * @brief find - Find element.
    *
    * @param [key] - Key of element.
    *
    * @returns  Element data pointer. If return NULL, you can use function getLastRet
    *           get return value.
    */
    T* find(std::string key) {
        _setret(ContainerRet::SUCCESS);
        if (key.empty() || key.size() > TT_max_key_length) {
            _setret(ContainerRet::TT_EKEY);
            return NULL;
        }
        TTNode* curnode = root_;
        unsigned int curoffset = 0;
        for (auto it : key) {
            curoffset = TT_DICT_INDEX(it);
            if (curoffset > TT_DICT_SIZE) {
                _setret(ContainerRet::TT_EINDEX);
                return NULL;
            }
            if (!curnode->slots_[curoffset]) {
                _setret(ContainerRet::TT_ENOTFOUND);
                return NULL;
            }
            curnode = curnode->slots_[curoffset];
        }
        return curnode->data_;
        
    }

    /**
    * @brief insert - Insert an element.
    *
    * @tparam [Args] - Template of element construct function arguments
    * @param [key] - Key of element.
    * @param [args] - Element's construct function arguments.
    *
    * @returns  Element data pointer. If return NULL, you can use function getLastRet
    *           get return value.
    */
    template<typename ... Args>
    T* insert(std::string key, Args&& ... args) {
        _setret(ContainerRet::SUCCESS);
        if (key.empty() || key.size() > TT_max_key_length) {
            _setret(ContainerRet::TT_EKEY);
            return NULL;
        }
        TTNode* curnode = root_;
        unsigned int curlevel = 0;
        unsigned int curoffset = 0;
        for (auto it : key) {
            curlevel++;
            curoffset = TT_DICT_INDEX(it);
            if (curoffset > TT_DICT_SIZE) {
                _setret(ContainerRet::TT_EINDEX);
                return NULL;
            }
            if (!curnode->slots_[curoffset]) {
                curnode->slots_[curoffset] = reinterpret_cast<struct TTNode*>(mp_->Malloc(sizeof(struct TTNode)));
                if (!(curnode->slots_[curoffset])) {
                    //Clean up empty nodes on the path
                    _TTNode_clean_reverse(curnode);
                    _setret(ContainerRet::EMALLOC);
                    return NULL;
                }
                _TTNode_init(curnode->slots_[curoffset]);
                curnode->slots_[curoffset]->level_ = curlevel;
                curnode->slots_[curoffset]->offset_ = curoffset;
                curnode->slots_[curoffset]->data_ = NULL;
                curnode->slots_[curoffset]->parent_ = curnode;
            }
            curnode = curnode->slots_[curoffset];
        }

        if (!(curnode->data_)) {
            curnode->data_ = reinterpret_cast<T*>(mp_->Malloc(sizeof(T)));
            mempool::Mempool::Construct<T>(curnode->data_, std::forward<Args>(args)...);
        } else {
            mempool::Mempool::Destruct<T>(curnode->data_);
            mempool::Mempool::Construct<T>(curnode->data_, std::forward<Args>(args)...);
        }

        if (!(curnode->data_)) {
            //Clean up empty nodes on the path
            _TTNode_clean_reverse(curnode);
            _setret(ContainerRet::EMALLOC);
        }
        count_++;
        return curnode->data_;
    }

    /**
    * @brief remove - Remove an element.
    *
    * @param [key] - Key of element.
    *
    * @returns  ContainerRet.
    */
    ContainerRet remove(std::string key) {
        _setret(ContainerRet::SUCCESS);
        if (key.empty() || key.size() > TT_max_key_length) {
            _setret(ContainerRet::TT_EKEY);
            return ContainerRet::TT_EKEY;
        }
        TTNode* curnode = root_;
        unsigned int curoffset = 0;
        for (auto it : key) {
            curoffset = TT_DICT_INDEX(it);
            if (curoffset > TT_DICT_SIZE) {
            _setret(ContainerRet::TT_EINDEX);
                return ContainerRet::TT_EINDEX;
            }
            if (!curnode->slots_[curoffset]) {
            _setret(ContainerRet::TT_ENOTFOUND);
                return ContainerRet::TT_ENOTFOUND;
            }
            curnode = curnode->slots_[curoffset];
        }
        if (!curnode->data_) {
            _setret(ContainerRet::TT_ENOTFOUND);
            return ContainerRet::TT_ENOTFOUND;
        } else {
            _TTNode_delete_data(curnode);
        }
        _setret(ContainerRet::SUCCESS);
        return ContainerRet::SUCCESS;
    }

    /**
    * @brief empty - Clean up all element in the TrieTree.
    */
    void empty() {
        for (auto it : root_->slots_) {
            _TTNode_delete_tree(it);
        }
    }

    /**
    * @brief display - Display TrieTree like "key --- data address".
    */
    void display() {
        _TTNode_print_tree(root_);
    }

private:
    //limmit
    unsigned int TT_max_key_length; ///< Key maximum length limit.

    mempool::Mempool* mp_;              ///< Mempool interface class pointer.
    unsigned int count_;            ///< Current count of elements.
    struct TTNode* root_;           ///< Root node pointer.
    ContainerRet last_return_;       ///< Last return value.

    /**
    * @brief _setret - Set last return value.
    *
    * @param [ret] - ContainerRet.
    */
    void _setret(ContainerRet ret) { last_return_ = ret; }

    void _TTNode_init(struct TTNode* node) {
        node->level_ = -1;
        node->offset_ = 0;
        node->data_ = NULL;
        node->parent_ = NULL;
        for (auto &it : node->slots_) {
            it = NULL;
        }
    }

    /**
    * @brief _TTNode_delete_data - Delete current node element data.
    *
    * @param [node] - Node pointer.
    */
    void _TTNode_delete_data(struct TTNode* node) {
        if (!node) {
            return;
        }
        mempool::Mempool::Destruct<T>(node->data_);
        mp_->Free(node->data_);
        node->data_ = NULL;
        _TTNode_clean_reverse(node);
        count_--;
    }

    /**
    * @brief _TTNode_delete_tree - Delete current node tree.
    *
    * @param [node] - Node pointer.
    */
    void _TTNode_delete_tree(struct TTNode* node) {
        if (!node) {
            return;
        }
        for (auto it : node->slots_) {
            _TTNode_delete_tree(it);
        }
        if (node->data_) {
            mempool::Mempool::Destruct<T>(node->data_);
            mp_->Free(node->data_);
            node->data_ = NULL;
            count_--;
        }
        if (node->parent_) {
            node->parent_->slots_[node->offset_] = NULL;
        }
        mp_->Free(node);
    }

    /**
    * @brief _TTNode_clean_reverse - Reverse clean unuse slots memory
    *                                marked by current node.
    *
    * @param [node] - Node pointer.
    */
    void _TTNode_clean_reverse(struct TTNode* node) {
        if (!node) {
            return;
        }
        struct TTNode* curnode = node;
        while (curnode->parent_) {
            bool emptyflag = false;
            if (!curnode->data_) {
                emptyflag = true;
                for (auto it : curnode->slots_) {
                    if (it) {
                        emptyflag = false;
                        break;
                    }
                }
            }
            if (emptyflag) {
                struct TTNode* tmp = curnode;
                curnode->parent_->slots_[curnode->offset_] = NULL;
                curnode = curnode->parent_;
                mp_->Free(tmp);
            } else {
                curnode = curnode->parent_;
            }
        };
    }

    /**
    * @brief _TTNode_print_tree - Print all elements of node tree.
    *
    * @param [node] - Node pointer.
    */
    void _TTNode_print_tree(struct TTNode* node) {
        if (!node) {
            return;
        }
        if (node->data_) {
            std::string key = _TTNode_get_key(node);
            printf("%s -- %p\n", key.c_str(), node->data_);
        }
        struct TTNode* curnode = node;
        for (int loop = 0; loop < TT_DICT_SIZE; loop++) {
            if (curnode->slots_[loop]) {
                _TTNode_print_tree(curnode->slots_[loop]);
            }
        }
    }

    /**
    * @brief _TTNode_insert_tree - Insert a TT tree.
    *
    * @param [node] - Node pointer.
    */
    void _TTNode_insert_tree(struct TTNode* node) {
        if (!node) {
            return;
        }
        if (node->data_) {
            std::string key = _TTNode_get_key(node);
            insert(key, *(node->data_));
        }
        struct TTNode* curnode = node;
        for (int loop = 0; loop < TT_DICT_SIZE; loop++) {
            if (curnode->slots_[loop]) {
                _TTNode_insert_tree(curnode->slots_[loop]);
            }
        }
    }

    /**
    * @brief _TTNode_get_key - Get current node' key.
    *
    * @param [node] - Node pointer.
    *
    * @returns  Key.
    */
    std::string _TTNode_get_key(struct TTNode* node) {
        std::string key;
        key.clear();
        if (!node) {
            return key;
        }
        struct TTNode* curnode = node;
        while (curnode->parent_) {
            key.append(1, (static_cast<char>(TT_DICT_CHAR(curnode->offset_))));
            curnode = curnode->parent_;
        }
        return std::string(key.rbegin(), key.rend());
    }
};

}// namespace end

#endif
