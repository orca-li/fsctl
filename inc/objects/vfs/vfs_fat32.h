#ifndef __FILEUUIDccb92e9f744f4f189661fa14b45831e2
#define __FILEUUIDccb92e9f744f4f189661fa14b45831e2

#include "fs/fat.h"

#define FSC_INC_OBJECTS_FAT32_PUBLIC
#ifdef FSC_INC_OBJECTS_FAT32_PRIVATE
#define FSC_INC_OBJECTS_FAT32_PROTECTED
#endif
typedef struct FSCTL_FAT32_HANDLER_TYPE
{
#ifdef FSC_INC_OBJECTS_FAT32_PUBLIC
    void (*mount)(void);
    void (*list)(const char *path);
    void (*umount)(void);
#endif
#ifdef FSC_INC_OBJECTS_FAT32_PROTECTED
#endif
#ifdef FSC_INC_OBJECTS_FAT32_PRIVATE
#endif
} fsc_fat32_handler_t;

extern fsc_fat32_handler_t fscFat32Handler;

#endif /** __FILEUUIDccb92e9f744f4f189661fa14b45831e2 */
