/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 09:58
 * @file     : sock_log.h
 * @brief    : Socket log api header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __SOCK_LOG_H__
#define __SOCK_LOG_H__

#include <stdio.h>

#define SOCK_DEBUG(fmt, args...) \
    printf("%s[%d]: ", __func__, __LINE__); \
    printf(fmt, ##args); \
    printf("\n");

#define SOCK_LOG(fmt, args...) \
    printf("%s[%d]: ", __func__, __LINE__); \
    printf(fmt, ##args); \
    printf("\n");

#define SOCK_ERROR(fmt, args...) \
    printf("%s[%d]: ", __func__, __LINE__); \
    printf(fmt, ##args); \
    printf("\n");

#endif
