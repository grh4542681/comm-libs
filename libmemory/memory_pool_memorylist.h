#ifndef __MEMPOOL_MEMORYLIST_H__
#define __MEMPOOL_MEMORYLIST_H__

namespace infra::mempool {

/**
* @brief - This linked list is used to store free memory blocks.
*          Use the head memory of the free memory block to store the next pointer.
*/
class MempoolMemoryList {
public:
    MempoolMemoryList() { head_ = NULL; }
    ~MempoolMemoryList() { }

    void Push(void* element) {
        void* next = head_;
        head_ = element;
        _SetNext(element, next);
    }

    void* Pop() {
        void* element = head_;
        head_ = _Next(head_);
        return element;
    }

    size_t Size() {
        int count = 0;
        void* tmp = head_;
        while (tmp) {
            count++;
            tmp = _Next(tmp);
        }
        return count;
    }
private:
    void* head_;

    MempoolMemoryList(MempoolMemoryList&) { }
    void* _Next(void* e) {
        return *(reinterpret_cast<void**>(e));
    }
    void _SetNext(void* e, void* n) {
        *(reinterpret_cast<void**>(e)) = n;
    }
};

} //namespace infra::end

#endif
