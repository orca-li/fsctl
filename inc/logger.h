#ifndef __FILEUUIDf730719652304a69a3fea56287d9911f
#define __FILEUUIDf730719652304a69a3fea56287d9911f

#include <stdio.h>

typedef struct FSCTL_LOGGER_TYPE
{
    // public
    int (* print)(const char *format, ...);
    int (* info)(const char *format, ...);
    int (* warning)(const char *format, ...);
    int (* error)(const char *format, ...);
    // private
    FILE *output;
    FILE *input;
} fsctl_logger_t;

extern fsctl_logger_t fscLogger;

#endif /** __FILEUUIDf730719652304a69a3fea56287d9911f */
