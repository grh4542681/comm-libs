#ifndef __MEMPOOL_CENTER_H__
#define __MEMPOOL_CENTER_H__

namespace mempool {

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


} //namespace end

#endif
