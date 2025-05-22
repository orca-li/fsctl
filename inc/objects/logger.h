#ifndef __FILEUUID2210f59bfca94244992d9aa570436e08
#define __FILEUUID2210f59bfca94244992d9aa570436e08

#include <stdio.h>
#include <stdbool.h>

#define FSC_INC_OBJECTS_LOGGER_PUBLIC
#ifdef FSC_INC_OBJECTS_LOGGER_PRIVATE
#define FSC_INC_OBJECTS_LOGGER_PROTECTED
#endif
typedef struct FSCTL_LOGGER_TYPE
{
#ifdef FSC_INC_OBJECTS_LOGGER_PUBLIC
    struct FSCTL_LOGGER_FLAGS_TYPE
    {
        bool verbose : 1;
    } flags;

    int (*print)(const char *format, ...);
    int (*info)(const char *format, ...);
    int (*warning)(const char *format, ...);
    int (*error)(const char *format, ...);
    int (*verbose)(const char *format, ...);
#endif
#ifdef FSC_INC_OBJECTS_LOGGER_PROTECTED
    FILE *output;
    FILE *input;
#endif
#ifdef FSC_INC_OBJECTS_LOGGER_PRIVATE
#endif
} fsctl_logger_t;

extern fsctl_logger_t fscLogger;

#endif /** __FILEUUID2210f59bfca94244992d9aa570436e08 */
