#ifndef __FILEUUIDccb92e9f744f4f189661fa14b45831e2
#define __FILEUUIDccb92e9f744f4f189661fa14b45831e2

typedef struct FSCTL_FAT32_HANDLER_TYPE
{
    // public
    void (* mount)(void);
    void (* list)(const char *path);
} fsc_fat32_handler_t;

extern fsc_fat32_handler_t fscFat32Handler;

#endif /** __FILEUUIDccb92e9f744f4f189661fa14b45831e2 */
