#ifndef __FILEUUID4cb19617833f4f8f895a03ec65643df7
#define __FILEUUID4cb19617833f4f8f895a03ec65643df7

#include "vfs_types.h"

typedef struct FSCTL_VFS_TYPE
{
    // public
    const char *path;
    void (*const list)(void);
    int (*const select_fs)(enum fsc_vfs_fs_t fs);
    void (*const mount)(void);
} fsc_vfs_t;

extern fsc_vfs_t fscVfs;

#endif /** __FILEUUID4cb19617833f4f8f895a03ec65643df7 */
