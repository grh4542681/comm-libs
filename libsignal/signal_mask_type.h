#ifndef __SIGNAL_MASK_TYPE_H__
#define __SIGNAL_MASK_TYPE_H__

#include <signal.h>

namespace infra::signal {

/**
* @brief - Signal mask type.
*/
enum class SignalMaskType {
    GETMASK = SIG_BLOCK - 1,    ///< Get current mask signal set.
    APPEND = SIG_BLOCK,         ///< Add new mask signal set.
    SUBTRACT = SIG_UNBLOCK,     ///< Delete mask signal set,
    REPLACE = SIG_SETMASK,      ///< Replace the original mask signal set.
};

}

#endif
