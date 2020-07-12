#ifndef __SHM_TRIE_TREE_H__
#define __SHM_TRIE_TREE_H__

#include "shm.h"

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

#define SHM_TT_DEFAULT_MAX_KEY_LENGTH (128) ///< Default max tire tree key lengtn.
#define SHM_TT_DICT_SIZE (95)               ///< Tire tree dictionary size.
#define SHM_TT_DICT_INDEX(c) (c & (~0x20))  ///< Calculate the offset of a character.
#define SHM_TT_DICT_CHAR(c) (c | (0x20))    ///< Calculate characters by offset.

namespace ipc::shm {

template < typename S, typename T >
class ShmTrieTree {
public:
    typedef struct _ShmTTNode {
        int level_ : 2;                         ///< current node level(length of the current key).
        unsigned int offset_ : 1;               ///< offset in parent node word slots.
        T* data_;                               ///< element data.
        ShmTTNode* parent_;                     ///< parent node pointer.
        ShmTTNode* slots_[SHM_TT_DICT_SIZE];    ///< next word slots.
    } ShmTTNode;

    typedef struct _ShmTTHead {
        std::string object_name_;
        size_t object_size_;
        size_t object_max_num_;
        size_t object_cur_num_;

        void* object_bitmap_;
        void* object_data_area_;

        ShmTTNode* root_;

        bool rw_lock_enable_;
        bool mutex_lock_enable_;

        timer::Time create_time_;
        timer::Time last_access_time_;
    } ShmTTHead;

public:
    ShmTrieTree(std::string path) : shm_(path) {
        static_assert(std::is_base_of<Shm, S>::value, "Must be a subclass that inherits from Shm.");
    }

    Shm& GetShm() {
        return shm_;
    }

    std::string GetObjectName() {
        std::string name = "";
        if (p_shm_head_) {
            name = p_shm_head_->object_name_;
        }
        return name;
    }

    size_t GetObjectSize() {
        if (p_shm_head_) {
            return p_shm_head_->object_size_;
        }
        return 0;
    }

    size_t GetObjectMaxNumber() {
        if (p_shm_head_) {
            return p_shm_head_->object_max_num_;
        }
        return 0;
    }

    size_t GetObjectCurNumber() {
        if (p_shm_head_) {
            return p_shm_head_->object_cur_num_;
        }
        return 0;
    }

    IpcRet Create(size_t minlen, size_t maxlen, mode_t mode) {
        size_t shm_size = sizeof(ShmListHead) + ((obj_num + 8 - 1) / 8)  + obj_num * (sizeof(T) + sizeof(ShmListNode));
        IpcRet ret = shm_.Create(shm_size, mode);
        if (ret != IpcRet::SUCCESS) {
            return ret;
        }
        ret = shm_.Open(IpcMode::READ_WRITE);
        if (ret == IpcRet::SUCCESS) {
            p_shm_head_ = reinterpret_cast<ShmListHead*>(shm_.GetHeadPtr());

            p_shm_head_->object_name_.assign(typeid(T).name());
            p_shm_head_->object_size_ = sizeof(T);
            p_shm_head_->object_max_num_ = obj_num;
            p_shm_head_->object_cur_num_ = 0;
            p_shm_head_->object_bitmap_ = reinterpret_cast<void*>(reinterpret_cast<char*>(shm_.GetHeadPtr()) + sizeof(ShmListHead));
            p_shm_head_->object_data_area_ = reinterpret_cast<char*>(shm_.GetHeadPtr()) + sizeof(ShmListHead) + (((obj_num + 64 - 1) / 64) * sizeof(long));
            p_shm_head_->object_head_ = NULL;
            p_shm_head_->object_tail_ = NULL;

            p_shm_head_->create_time_ = timer::Time::Now();
        }
        ret = shm_.Close();
        if (ret == IpcRet::SUCCESS) {
            p_shm_head_ = NULL;
        }

        return ret;
    }
    IpcRet Destroy() {
        IpcRet ret = shm_.Destroy();
        if (ret == IpcRet::SUCCESS) {
            p_shm_head_ = NULL;
        }
        return ret;
    }
    IpcRet Open(IpcMode mode) {
        IpcRet ret = shm_.Open(mode);
        if (ret == IpcRet::SUCCESS) {
            p_shm_head_ = reinterpret_cast<ShmListHead*>(shm_.GetHeadPtr());

            p_shm_head_->last_access_time_ = timer::Time::Now();
        }
        return ret;
    }
    IpcRet Close() {
        IpcRet ret = shm_.Close();
        if (ret == IpcRet::SUCCESS) {
            p_shm_head_ = NULL;
        }
        return ret;
    }


    TrieTree() {
        count_ = 0;
        SHM_TT_max_key_length = SHM_TT_DEFAULT_MAX_KEY_LENGTH;
        mp_ = mempool::Mempool::getInstance();        
        root_ = reinterpret_cast<ShmTTNode*>(mp_->Malloc(sizeof(ShmTTNode)));
        _ShmTTNode_init(root_);
    }

    TrieTree(unsigned int max_len) {
        count_ = 0;
        SHM_TT_max_key_length = max_len;
        mp_ = mempool::Mempool::getInstance();        
        root_ = reinterpret_cast<ShmTTNode*>(mp_->Malloc(sizeof(ShmTTNode)));
        _ShmTTNode_init(root_);
    }
    /**
    * @brief TrieTree - Copy construct function.
    *
    * @param [other] - Other instance.
    */
    TrieTree(TrieTree& other) {
        count_ = 0;
        SHM_TT_max_key_length = other.SHM_TT_max_key_length;
        mp_ = mempool::Mempool::getInstance();
        root_ = reinterpret_cast<ShmTTNode*>(mp_->Malloc(sizeof(ShmTTNode)));
        _ShmTTNode_init(root_);
        _ShmTTNode_insert_tree(other.getRoot());
    }
    /**
    * @brief ~TrieTree - Destruct function.
    */
    ~TrieTree() {
        _ShmTTNode_delete_tree(root_);
    }

    /**
    * @brief getMaxKeyLen - Get the max key length in this TrieTree.
    *
    * @returns  Key maximum length limit.
    */
    unsigned int getMaxKeyLen() { return SHM_TT_max_key_length; }
    /**
    * @brief getCount -Get count of elements int this TrieTree now. 
    *
    * @returns  Element counts.
    */
    unsigned int getCount() { return count_; }

    ShmTTNode* getRoot() { return root_; }

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
        if (key.empty() || key.size() > SHM_TT_max_key_length) {
            _setret(ContainerRet::SHM_TT_EKEY);
            return NULL;
        }
        ShmTTNode* curnode = root_;
        unsigned int curoffset = 0;
        for (auto it : key) {
            curoffset = SHM_TT_DICT_INDEX(it);
            if (curoffset > SHM_TT_DICT_SIZE) {
                _setret(ContainerRet::SHM_TT_EINDEX);
                return NULL;
            }
            if (!curnode->slots_[curoffset]) {
                _setret(ContainerRet::SHM_TT_ENOTFOUND);
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
        if (key.empty() || key.size() > SHM_TT_max_key_length) {
            _setret(ContainerRet::SHM_TT_EKEY);
            return NULL;
        }
        ShmTTNode* curnode = root_;
        unsigned int curlevel = 0;
        unsigned int curoffset = 0;
        for (auto it : key) {
            curlevel++;
            curoffset = SHM_TT_DICT_INDEX(it);
            if (curoffset > SHM_TT_DICT_SIZE) {
                _setret(ContainerRet::SHM_TT_EINDEX);
                return NULL;
            }
            if (!curnode->slots_[curoffset]) {
                curnode->slots_[curoffset] = reinterpret_cast<ShmTTNode*>(mp_->Malloc(sizeof(ShmTTNode)));
                if (!(curnode->slots_[curoffset])) {
                    //Clean up empty nodes on the path
                    _ShmTTNode_clean_reverse(curnode);
                    _setret(ContainerRet::EMALLOC);
                    return NULL;
                }
                _ShmTTNode_init(curnode->slots_[curoffset]);
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
            _ShmTTNode_clean_reverse(curnode);
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
        if (key.empty() || key.size() > SHM_TT_max_key_length) {
            _setret(ContainerRet::SHM_TT_EKEY);
            return ContainerRet::SHM_TT_EKEY;
        }
        ShmTTNode* curnode = root_;
        unsigned int curoffset = 0;
        for (auto it : key) {
            curoffset = SHM_TT_DICT_INDEX(it);
            if (curoffset > SHM_TT_DICT_SIZE) {
            _setret(ContainerRet::SHM_TT_EINDEX);
                return ContainerRet::SHM_TT_EINDEX;
            }
            if (!curnode->slots_[curoffset]) {
            _setret(ContainerRet::SHM_TT_ENOTFOUND);
                return ContainerRet::SHM_TT_ENOTFOUND;
            }
            curnode = curnode->slots_[curoffset];
        }
        if (!curnode->data_) {
            _setret(ContainerRet::SHM_TT_ENOTFOUND);
            return ContainerRet::SHM_TT_ENOTFOUND;
        } else {
            _ShmTTNode_delete_data(curnode);
        }
        _setret(ContainerRet::SUCCESS);
        return ContainerRet::SUCCESS;
    }

    /**
    * @brief empty - Clean up all element in the TrieTree.
    */
    void empty() {
        for (auto it : root_->slots_) {
            _ShmTTNode_delete_tree(it);
        }
    }

    /**
    * @brief display - Display TrieTree like "key --- data address".
    */
    void display() {
        _ShmTTNode_print_tree(root_);
    }

private:
    //limmit
    unsigned int SHM_TT_max_key_length; ///< Key maximum length limit.

    mempool::Mempool* mp_;              ///< Mempool interface class pointer.
    unsigned int count_;            ///< Current count of elements.
    ShmTTNode* root_;           ///< Root node pointer.
    ContainerRet last_return_;       ///< Last return value.

    /**
    * @brief _setret - Set last return value.
    *
    * @param [ret] - ContainerRet.
    */
    void _setret(ContainerRet ret) { last_return_ = ret; }

    void _ShmTTNode_init(ShmTTNode* node) {
        node->level_ = -1;
        node->offset_ = 0;
        node->data_ = NULL;
        node->parent_ = NULL;
        for (auto &it : node->slots_) {
            it = NULL;
        }
    }

    /**
    * @brief _ShmTTNode_delete_data - Delete current node element data.
    *
    * @param [node] - Node pointer.
    */
    void _ShmTTNode_delete_data(ShmTTNode* node) {
        if (!node) {
            return;
        }
        mempool::Mempool::Destruct<T>(node->data_);
        mp_->Free(node->data_);
        node->data_ = NULL;
        _ShmTTNode_clean_reverse(node);
        count_--;
    }

    /**
    * @brief _ShmTTNode_delete_tree - Delete current node tree.
    *
    * @param [node] - Node pointer.
    */
    void _ShmTTNode_delete_tree(ShmTTNode* node) {
        if (!node) {
            return;
        }
        for (auto it : node->slots_) {
            _ShmTTNode_delete_tree(it);
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
    * @brief _ShmTTNode_clean_reverse - Reverse clean unuse slots memory
    *                                marked by current node.
    *
    * @param [node] - Node pointer.
    */
    void _ShmTTNode_clean_reverse(ShmTTNode* node) {
        if (!node) {
            return;
        }
        ShmTTNode* curnode = node;
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
                ShmTTNode* tmp = curnode;
                curnode->parent_->slots_[curnode->offset_] = NULL;
                curnode = curnode->parent_;
                mp_->Free(tmp);
            } else {
                curnode = curnode->parent_;
            }
        };
    }

    /**
    * @brief _ShmTTNode_print_tree - Print all elements of node tree.
    *
    * @param [node] - Node pointer.
    */
    void _ShmTTNode_print_tree(ShmTTNode* node) {
        if (!node) {
            return;
        }
        if (node->data_) {
            std::string key = _ShmTTNode_get_key(node);
            printf("%s -- %p\n", key.c_str(), node->data_);
        }
        ShmTTNode* curnode = node;
        for (int loop = 0; loop < SHM_TT_DICT_SIZE; loop++) {
            if (curnode->slots_[loop]) {
                _ShmTTNode_print_tree(curnode->slots_[loop]);
            }
        }
    }

    /**
    * @brief _ShmTTNode_insert_tree - Insert a TT tree.
    *
    * @param [node] - Node pointer.
    */
    void _ShmTTNode_insert_tree(ShmTTNode* node) {
        if (!node) {
            return;
        }
        if (node->data_) {
            std::string key = _ShmTTNode_get_key(node);
            insert(key, *(node->data_));
        }
        ShmTTNode* curnode = node;
        for (int loop = 0; loop < SHM_TT_DICT_SIZE; loop++) {
            if (curnode->slots_[loop]) {
                _ShmTTNode_insert_tree(curnode->slots_[loop]);
            }
        }
    }

    /**
    * @brief _ShmTTNode_get_key - Get current node' key.
    *
    * @param [node] - Node pointer.
    *
    * @returns  Key.
    */
    std::string _ShmTTNode_get_key(ShmTTNode* node) {
        std::string key;
        key.clear();
        if (!node) {
            return key;
        }
        ShmTTNode* curnode = node;
        while (curnode->parent_) {
            key.append(1, (static_cast<char>(SHM_TT_DICT_CHAR(curnode->offset_))));
            curnode = curnode->parent_;
        }
        return std::string(key.rbegin(), key.rend());
    }
             
private:
    S shm_;
    ShmTTHead* p_shm_head_;
};

}

#endif
