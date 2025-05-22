#ifndef __FILEUUID336311bba9844c79bc9ac07445b18486
#define __FILEUUID336311bba9844c79bc9ac07445b18486

typedef struct FSCTL_INOUT_IMAGE_TYPE
{
    FILE *handler;
    void (* const read)(const char *filename);
    void (* const close)(void);
} fsctl_inout_image_t;

typedef struct FSCTL_INOUT_TYPE
{
    // public
    fsctl_inout_image_t image;
} fsctl_inout_t;

extern fsctl_inout_t fscIO;

#endif /** __FILEUUID336311bba9844c79bc9ac07445b18486 */
