#ifndef __UTILS_H__
#define __UTILS_H__ 

#include <stdio.h>
#include "vocli.h"

extern FILE *fopenf(const char *mode, const char *format, ...)
    __attribute__((format(printf, 2, 3)));

/* String utilities */
extern char *strstrip(char *str, char *trail);
extern char *strchomp(char *str);
extern char *strtofile(const char *str, const char *ext);

#endif//__UTILS_H__
