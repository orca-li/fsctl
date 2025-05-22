#ifndef __FILEUUID4cb19617833f4f8f895a03ec65643df7
#define __FILEUUID4cb19617833f4f8f895a03ec65643df7

enum fsc_vfs_fs_t
{
    fsc_vfs_error,
    fsc_vfs_fat32,
};

#define FSC_INC_OBJECTS_VFS_PUBLIC
#ifdef FSC_INC_OBJECTS_VFS_PRIVATE
#define FSC_INC_OBJECTS_VFS_PROTECTED
#endif
typedef struct FSCTL_VFS_TYPE
{
#ifdef FSC_INC_OBJECTS_VFS_PUBLIC
    const char *path;
    void (*const list)(void);
    int (*const select_fs)(enum fsc_vfs_fs_t fs);
    void (*const mount)(void);
#endif
#ifdef FSC_INC_OBJECTS_VFS_PROTECTED
    void (*list_link)(const char *path);
    void (*mount_link)(void);
    enum fsc_vfs_fs_t current_fs;
#endif
#ifdef FSC_INC_OBJECTS_VFS_PRIVATE
    const char *pwd;
#endif
} fsc_vfs_t;

extern fsc_vfs_t fscVfs;

#endif /** __FILEUUID4cb19617833f4f8f895a03ec65643df7 */
