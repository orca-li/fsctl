#ifndef __FILEUUID74e536001fc640d4b7305076a273b83d
#define __FILEUUID74e536001fc640d4b7305076a273b83d

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
#endif
#ifdef FSC_INC_OBJECTS_OPTION_PRIVATE
    void (*const set_main_line)(int line);
    int (*const check_main_line)(void);
#endif
} fsctl_option_t;

extern fsctl_option_t fscOption;

#endif /** __FILEUUID74e536001fc640d4b7305076a273b83d */
