#ifndef __MEMPOOL_CENTER_H__
#define __MEMPOOL_CENTER_H__

namespace infra::mempool {

class MempoolCenter {
public:
    class CenterCache {

    };
    
public:
    static MempoolCenter* pInstance;

    void report();
    static MempoolCenter* getInstance();
private:
    MempoolCenter();
    ~MempoolCenter();
};


} //namespace infra::end

#endif
