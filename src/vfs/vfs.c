#include <stdlib.h>
#define FSC_INC_OBJECTS_VFS_PRIVATE
#include "objects.h"
#include "public.h"

static void VfsMount(void);
static int SelectFileSystem(enum fsc_vfs_fs_t fs);
static void VfsList(void);

fsc_vfs_t fscVfs = {
    // public
    .path = NULL,
    .list = VfsList,
    .select_fs = SelectFileSystem,
    .mount = VfsMount,
    // protected
    .list_link = NULL,
    .mount_link = NULL,
    .current_fs = fsc_vfs_fat32,
    // private
    .pwd = "/",
};

static int SelectFileSystem(enum fsc_vfs_fs_t fs)
{
    switch (fs)
    {
    case fsc_vfs_fat32:
        fscVfs.current_fs = fs;
        fscVfs.list_link = fscFat32Handler.list;
        fscVfs.mount_link = fscFat32Handler.mount;
        break;

    default:
        fscVfs.current_fs = fsc_vfs_error;
        return EXIT_FAILURE;
    }

    if (fscVfs.list_link != NULL
    && fscVfs.mount_link != NULL)
        return EXIT_SUCCESS;

    fscLogger.error("File system selection error");
    exit(EXIT_FAILURE);
}

static void VfsMount(void)
{
    fscVfs.mount_link();
}

static void VfsList(void)
{
    if (fscVfs.path == NULL)
    {
        fscVfs.list_link(fscVfs.pwd);
        return;
    }

    fscVfs.list_link(fscVfs.path);
}
