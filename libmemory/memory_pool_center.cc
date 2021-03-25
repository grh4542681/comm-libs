#include <stdio.h>
#include "mempool_center.h"

namespace infra::mempool {

MempoolCenter* MempoolCenter::pInstance = NULL;

MempoolCenter::MempoolCenter() {

}

MempoolCenter::~MempoolCenter() {

}

MempoolCenter* MempoolCenter::getInstance() {
    if (!pInstance) {
        pInstance = new MempoolCenter();
    }
    return pInstance;
}

} //namespace infra::end
