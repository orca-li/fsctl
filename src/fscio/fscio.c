#include <stdio.h>
#include "public.h"
#include "logger.h"

typedef struct FSCTL_INOUT_IMAGE_TYPE
{
    FILE *handler;
    void (*read)(const char *filename);
    void (*close)(void);
} fsctl_inout_image_t;

typedef struct FSCTL_INOUT_TYPE
{
    // public
    fsctl_inout_image_t image;
} fsctl_inout_t;

static void ReadImageFile(const char *filename);
static void CloseImageFile(void);

fsctl_inout_t fscIO = {
    .image = {
        .handler = NULL,
        .read = ReadImageFile,
        .close = CloseImageFile,
    }
};

static void ReadImageFile(const char *filename)
{
    fscIO.image.handler = fopen(filename, "rb");
    if (fscIO.image.handler == NULL)
    {
        fscLogger.error("Error opening image %s", filename);
        exit(EXIT_FAILURE);
    }
}

static void CloseImageFile(void)
{
    fclose(fscIO.image.handler);
    fscIO.image.handler = NULL;
}