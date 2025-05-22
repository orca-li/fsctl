#ifndef __FILEUUID56d34cd91fb64475a7400f58a3fb7b46
#define __FILEUUID56d34cd91fb64475a7400f58a3fb7b46

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

extern fsctl_getinfo_t fscGetInfo;

#endif /** __FILEUUID56d34cd91fb64475a7400f58a3fb7b46 */
