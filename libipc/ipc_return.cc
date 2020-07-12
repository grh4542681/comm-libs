#include "ipc_return.h"

namespace ipc {

IpcRet::ECodeMapType IpcRet::ECodeMap = {
    // fifo return val
    { ECode::FIFO_ESOCKPAIR, "" },

    // shm return val
    { ECode::SHM_EMODE, "" },
    { ECode::SHM_ECREATED, "" },

    // sem return val
    { ECode::SEM_EMODE, "" },
    { ECode::SEM_ENOTFOUND, "" },

    // sem rw lock
    { ECode::SEM_ELOCKED, "" },

    // shm list return val
    { ECode::SL_ENOSPACE, "" },
    { ECode::SL_EBITMAP, "" },

    // msg shm return val
    { ECode::MSG_ENOSPACE, "" }
};

}
