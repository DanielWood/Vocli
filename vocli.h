#ifndef __VOCLI_H__
#define __VOCLI_H__

#include <stdbool.h>

// Make an info.h?
extern const char *vocli_version;
extern const char *author_email;

// Return flags
#define SUCCESS 0    // Routine completed successfully
#define FAIL 1       // Routine failed to complete


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
extern void die(int retval, const char *format, ...)
    __attribute__((noreturn, format(printf, 2, 3)));

#endif//__VOCLI_H__
