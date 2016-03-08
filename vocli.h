#ifndef __VOCLI_H__
#define __VOCLI_H__

#include "config.h"

// Return flags
#define SUCCESS 0
#define FAIL 1

#ifndef __GNUC__
    #define __attribute__(x)
#endif


// Logging
enum {  LL_MSG = 0,
        LL_ERR,
        LL_WARN };

extern void vocli_log(int level, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

// Error handling
extern int error(int retval, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

#endif//__VOCLI_H__
