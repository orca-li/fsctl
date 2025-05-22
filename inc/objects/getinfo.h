#ifndef __FILEUUID9e526a81f5d84b04b5ff512cfdd3e16f
#define __FILEUUID9e526a81f5d84b04b5ff512cfdd3e16f

typedef struct FSCTL_GETINFO_PROGRAM_TYPE
{
    const char *const name;
    const char *path;
    const char *const vendor;
} fsctl_getinfo_program_t;

typedef struct FSCTL_GETINFO_VERSION_TYPE
{
    const int year;
    const char *const string;
    const int major;
    const int minor;
    const int patch;
} fsctl_getinfo_version_t;

#define FSC_INC_OBJECTS_GETINFO_PUBLIC
#ifdef FSC_INC_OBJECTS_GETINFO_PRIVATE
#define FSC_INC_OBJECTS_GETINFO_PROTECTED
#endif

typedef struct FSCTL_GETINFO_TYPE
{
#ifdef FSC_INC_OBJECTS_GETINFO_PUBLIC
    fsctl_getinfo_program_t program;
    fsctl_getinfo_version_t version;
#endif
#ifdef FSC_INC_OBJECTS_GETINFO_PROTECTED
#endif
#ifdef FSC_INC_OBJECTS_GETINFO_PRIVATE
#endif
} fsctl_getinfo_t;

extern fsctl_getinfo_t fscGetInfo;

#endif /** __FILEUUID9e526a81f5d84b04b5ff512cfdd3e16f */
