#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "public.h"

typedef struct FSCTL_LOGGER_TYPE
{
    // public
    int (*print)(const char *format, ...);
    int (*info)(const char *format, ...);
    int (*warning)(const char *format, ...);
    int (*error)(const char *format, ...);
    // private
    FILE *output;
    FILE *input;
} fsctl_logger_t;

static int Print(const char *format, ...);
static int PrintInfo(const char *format, ...);
static int PrintWarning(const char *format, ...);
static int PrintError(const char *format, ...);
static int Print(const char *format, ...);

fsctl_logger_t fscLogger = {
    .print = Print,
    .info = PrintInfo,
    .warning = PrintWarning,
    .error = PrintError,
    .output = NULL,
    .input = NULL,
};

static int PrintTagged(const char *tag, const char *format, va_list args)
{
    int result = 0;
    char *buffer = NULL;
    int tag_len = strlen(tag);
    int required_size;

    va_list args_copy;
    va_copy(args_copy, args);
    required_size = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (required_size < 0)
    {
        return -1;
    }

    buffer = (char *)malloc(tag_len + required_size + nullterm);
    if (buffer == NULL)
    {
        return -1;
    }

    memcpy(buffer, tag, tag_len);
    buffer[tag_len] = '\0';

    result = vsnprintf(buffer + tag_len, required_size + nullterm, format, args);

    if (result < 0 || result > required_size)
    {
        free(buffer);
        return -1;
    }

    result = fprintf(fscLogger.output, "%s", buffer);
    free(buffer);

    return result;
}
static int PrintInfo(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = PrintTagged("[INFO] ", format, args);
    va_end(args);
    return result;
}

static int PrintWarning(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = PrintTagged("[WARNING] ", format, args);
    va_end(args);
    return result;
}

static int PrintError(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = PrintTagged("[ERROR] ", format, args);
    va_end(args);
    return result;
}

static int Print(const char *format, ...)
{
    va_list args;
    int result;

    va_start(args, format);
    result = vfprintf(fscLogger.output, format, args);
    va_end(args);

    return result;
}
