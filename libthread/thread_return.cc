#include "thread_return.h"

namespace thread {

ThreadRet::ECodeMapType ThreadRet::ECodeMap = {
    { THREAD_EBASE, "" },
    { THREAD_EREGISTER, "" },
    { THREAD_EUNREGISTER, "" },
    { THREAD_ERUNNING, "" },
    { THREAD_ESIGNALMASKTYPE, "" },
    { THREAD_ESIGNALMASK, "" },
};

}
