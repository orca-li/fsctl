#include <stdio.h>
#define FSC_INC_OBJECTS_FSCIO_PRIVATE
#include "objects.h"
#include "public.h"

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