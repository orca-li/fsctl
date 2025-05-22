#include "public.h"

#define VENDOR_NAME "Orcali"
#define VERSION_YEAR 2025u
#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 0
#if VERSION_PATCH != 0
#define VERSION_STRING TO_STRING(VERSION_MAJOR.VERSION_MINOR.VERSION_PATCH)
#else
#define VERSION_STRING TO_STRING(VERSION_MAJOR.VERSION_MINOR)
#endif

typedef struct FSCTL_GETINFO_PROGRAM_TYPE {
    const char * const name;
    const char * path;
    const char * const vendor;
} fsctl_getinfo_program_t;

typedef struct FSCTL_GETINFO_VERSION_TYPE {
    const int year;
    const char * const string;
    const int major;
    const int minor;
    const int patch;
} fsctl_getinfo_version_t;

typedef struct FSCTL_GETINFO_TYPE {
    fsctl_getinfo_program_t program;
    fsctl_getinfo_version_t version;
} fsctl_getinfo_t;

fsctl_getinfo_t fscGetInfo = {
    .program = {
        .name = "fsctl",
        .path = NULL,
        .vendor = VENDOR_NAME,
    },
    .version = {
        .year = VERSION_YEAR,
        .string = VERSION_STRING,
        .major = VERSION_MAJOR,
        .minor = VERSION_MINOR,
        .patch = VERSION_PATCH,
    },
};
