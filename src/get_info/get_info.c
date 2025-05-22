#define FSC_INC_OBJECTS_GETINFO_PRIVATE
#include "public.h"
#include "objects.h"

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
