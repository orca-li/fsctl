#ifndef __FILEUUID336311bba9844c79bc9ac07445b18486
#define __FILEUUID336311bba9844c79bc9ac07445b18486

#include <stdio.h>

typedef struct FSCTL_INOUT_IMAGE_TYPE
{
    FILE *handler;
    void (*const read)(const char *filename);
    void (*const close)(void);
} fsctl_inout_image_t;

#define FSC_INC_OBJECTS_FSCIO_PUBLIC
#ifdef FSC_INC_OBJECTS_FSCIO_PRIVATE
#define FSC_INC_OBJECTS_FSCIO_PROTECTED
#endif

typedef struct FSCTL_INOUT_TYPE
{
#ifdef FSC_INC_OBJECTS_FSCIO_PUBLIC
    fsctl_inout_image_t image;
#endif
#ifdef FSC_INC_OBJECTS_FSCIO_PROTECTED
#endif
#ifdef FSC_INC_OBJECTS_FSCIO_PRIVATE
#endif
} fsctl_inout_t;

extern fsctl_inout_t fscIO;

#endif /** __FILEUUID336311bba9844c79bc9ac07445b18486 */
