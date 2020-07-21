/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-04-18 04:16
 * @file     : process_signal_mask_type.h
 * @brief    : Signal mask type.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PROCESS_SIGNAL_MASK_TYPE_H__
#define __PROCESS_SIGNAL_MASK_TYPE_H__

#include <signal.h>

namespace process::signal {

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
