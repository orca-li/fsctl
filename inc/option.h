#ifndef __FILEUUID74e536001fc640d4b7305076a273b83d
#define __FILEUUID74e536001fc640d4b7305076a273b83d

typedef struct FSCTL_OPTION_TYPE {
    int (* const getting)(int argc, char **argv);
    void (* const call)(void);
} fsctl_option_t;

extern fsctl_option_t fscOption;

#endif /** __FILEUUID74e536001fc640d4b7305076a273b83d */
