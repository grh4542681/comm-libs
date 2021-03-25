#ifndef __SHM_AVL_TREE_H__
#define __SHM_AVL_TREE_H__

namespace infra::ipc::shm {

template < typename S, typename T>
class ShmAvlTree {
public:
    ShmAvlTree();
    ~ShmAvlTree();

private:
    S shm_;
};

}

#endif