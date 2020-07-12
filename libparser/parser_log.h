/*******************************************************
 * Copyright (C) For free.
 * All rights reserved.
 *******************************************************
 * @author   : Ronghua Gao
 * @date     : 2019-03-04 09:02
 * @file     : parser_log.h
 * @brief    : Parser log api header.
 * @note     : Email - grh4542681@163.com
 * ******************************************************/
#ifndef __PARSER_LOG_H__
#define __PARSER_LOG_H__

#include <stdio.h>

#define PARSER_DEBUG(fmt, args...) \
    printf("%s[%d]: ", __func__, __LINE__); \
    printf(fmt, ##args); \
    printf("\n");

#define PARSER_LOG(fmt, args...) \
    printf("%s[%d]: ", __func__, __LINE__); \
    printf(fmt, ##args); \
    printf("\n");

#define PARSER_ERROR(fmt, args...) \
    printf("%s[%d]: ", __func__, __LINE__); \
    printf(fmt, ##args); \
    printf("\n");

#endif
