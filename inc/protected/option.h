#ifndef __FILEUUIDa13addbc8a2b4b1eb97fcd14fb724eec
#define __FILEUUIDa13addbc8a2b4b1eb97fcd14fb724eec

typedef struct FSCTL_OPTION_TYPE
{
    // public
    int (*const getting)(int argc, char **argv);
    void (*call)(void);
    // protected
    const char *input_image;
    volatile int const main_line;
} fsctl_option_t;

extern fsctl_option_t fscOption;

#endif /** __FILEUUIDa13addbc8a2b4b1eb97fcd14fb724eec */
