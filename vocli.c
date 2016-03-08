#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include "vocli.h"
#include "utils.h"

// Wrapper for va_vocli_log
void vocli_log(int level, const char *format, ...)
{
#ifdef VOCLI_LOG_ENABLE
    va_list args;
    va_start(args, format);
    va_vocli_log(level, format, args);
    va_end(args);
#endif
}

// Write to log
void va_vocli_log(int level, const char *format, va_list args)
{
#ifdef VOCLI_LOG_ENABLE
    time_t t = time(0);
    
    static char logname[128];
    if (!*logname)
        snprintf(logname, 128, "vocli_log_%d.log", time);

    FILE *logfh = fopen(logname, "a");
    if (logfh == NULL)
        logfh = stderr;
   
    static bool is_initialized = false; 
    if (!is_initialized)
    {
        fprintf(logfh,
                "Vocli %s debug log (%s)\n",
                "-----------------------------------------------+\n\n",
                vocli_version, strchomp(ctime(&t)));
        is_initialized = true;
    }

    switch (level)
    {
        case LL_ERR:
            fprintf(logfh, "Error: ");
            break;
        case LL_WARN:
            fprintf(logfh, "Warning: ");
            break;
    }

    vfprintf(logfh, format, args);
    fputc('\n', logfh);

    fclose(logfh);
#endif
}

// Error handling
int error(int retval, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Write to stderr
    vfprintf(stderr, format, args);
    fputc('\n', stderr);

    va_vocli_log(retval, format, args);

    va_end(args);
    return retval;
}

