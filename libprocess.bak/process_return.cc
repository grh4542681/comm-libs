#include "process_return.h"

namespace infra::process {

ProcessRet::ECodeMapType ProcessRet::ECodeMap = {
    { PROCESS_EMEMORY, "" },
    { PROCESS_EACCES, "" },
    { PROCESS_EFAULT, "" },
    { PROCESS_EINVAL, "" },
    { PROCESS_EIO, "" },
    { PROCESS_ELOOP, "" },
    { PROCESS_ENAMETOOLONG, "" },
    { PROCESS_ENOENT, "" },
    { PROCESS_ENOMEM, "" },
    { PROCESS_ENOTDIR, "" },
    { PROCESS_ETHREADDUP, "" },
    { PROCESS_ETHREADADD, "" },
    { PROCESS_ETHREADNOTFOUND, "" },
    { PROCESS_EPROCDUP, "" },
    { PROCESS_EPROCADD, "" },
    { PROCESS_EPROCNOTFOUND, "" },
    { PROCESS_ECMDLINE, "" },
    { PROCESS_ESIGINVAL, "" },
    { PROCESS_EADDSIGNAL, "" },
    { PROCESS_ESIGNALREG, "" },
    { PROCESS_ESIGNALNOTFOUND, "" },
    { PROCESS_ESIGNALMASK, "" },
    { PROCESS_ESIGNALMASKTYPE, "" },
    { PROCESS_ECALLABLE, "" },
    { PROCESS_EFORK, "" },
    { PROCESS_EFIFOPAIR, "" },
};

}
