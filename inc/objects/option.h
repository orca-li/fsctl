#ifndef __FILEUUID74e536001fc640d4b7305076a273b83d
#define __FILEUUID74e536001fc640d4b7305076a273b83d

#include <stdbool.h>

typedef struct FSCTL_OPTION_FLAGS_LIST_TYPE
{
    bool fmtlong:1;
    bool fmtmax:1;
} fsctl_option_f_list_t;

#define FSC_INC_OBJECTS_OPTION_PUBLIC
#ifdef FSC_INC_OBJECTS_OPTION_PRIVATE
#define FSC_INC_OBJECTS_OPTION_PROTECTED
#endif
typedef struct FSCTL_OPTION_TYPE
{
#ifdef FSC_INC_OBJECTS_OPTION_PUBLIC
    int (*const getting)(int argc, char **argv);
    void (*call)(void);
#endif
#ifdef FSC_INC_OBJECTS_OPTION_PROTECTED
    const char *input_image;
    volatile int const main_line;
    struct FSCTL_OPTION_FLAGS_TYPE
    {
        bool once:1;
        const char *string;
        fsctl_option_f_list_t list;
    } flags;
#endif
#ifdef FSC_INC_OBJECTS_OPTION_PRIVATE
    void (*const set_main_line)(int line);
    int (*const check_main_line)(void);
    void (*const bind_sub_flags)(const char *flags);
#endif
} fsctl_option_t;

extern fsctl_option_t fscOption;

#endif /** __FILEUUID74e536001fc640d4b7305076a273b83d */
