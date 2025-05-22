#ifndef __FILEUUID1c3c891456d547aba4b1f1d2e33a11d9
#define __FILEUUID1c3c891456d547aba4b1f1d2e33a11d9

#include <stdint.h>

typedef struct FSCTL_FAT32_BOOTSECTOR_T
{
    uint8_t jmpBoot[3];
    uint8_t OEMName[8];
    uint16_t BytsPerSec;
    uint8_t SecPerClus;
    uint16_t RsvdSecCnt;
    uint8_t NumFATs;
    uint16_t RootEntCnt;
    uint16_t TotSec16;
    uint8_t Media;
    uint16_t FATSz16;
    uint16_t SecPerTrk;
    uint16_t NumHeads;
    uint32_t HiddSec;
    uint32_t TotSec32;
    uint32_t FATSz32; // fat32
    uint16_t ExtFlags;
    uint16_t FSVer;
    uint32_t RootClus;
    uint16_t FSInfo;
    uint16_t BkBootSec;
    uint8_t Reserved[12];
    uint8_t DrvNum;
    uint8_t Reserved1[1];
    uint8_t BootSig;
    uint32_t VolID;
    uint8_t VolLab[11];
    uint8_t FilSysType[8];
} __attribute__((packed)) fscFat32BootSector_t;

typedef struct FSCTL_FAT_DIR_ENTRY_TYPE
{
    uint8_t Name[11];
    uint8_t Attr;
    uint8_t NTRes;
    uint8_t CrtTimeTenth;
    uint16_t CrtTime;
    uint16_t CrtDate;
    uint16_t LstAccDate;
    uint16_t FstClusHI;
    uint16_t WrtTime;
    uint16_t WrtDate;
    uint16_t FstClusLO;
    uint32_t FileSize;
} __attribute__((packed)) fscFatDir_t;

typedef struct FSCTL_FAT_LONG_DIR_ENTRY_TYPE
{
    uint8_t Ord;
    uint8_t Name1[10];
    uint8_t Attr;
    uint8_t Type;
    uint8_t Chksum;
    uint8_t Name2[12];
    uint16_t FstClusLO;
    uint8_t Name3[4];
} __attribute__((packed)) fscFatLongDir_t;

#define FSC_FAT_ATTR_READ_ONLY 0x01
#define FSC_FAT_ATTR_HIDDEN 0x02
#define FSC_FAT_ATTR_SYSTEM 0x04
#define FSC_FAT_ATTR_VOLUME_ID 0x08
#define FSC_FAT_ATTR_DIRECTORY 0x10
#define FSC_FAT_ATTR_ARCHIVE 0x20
#define FSC_FAT_ATTR_LONG_NAME 0x0F

#define FSC_FAT_CLUSTER_AVAILABLE 0x00000000
#define FSC_FAT_CLUSTER_RESERVED_MIN 0xFFFFFF00
#define FSC_FAT_CLUSTER_RESERVED_MAX 0xFFFFFF0F
#define FSC_CLUSTER_IS_RESERVED(cluster_value) \
    ((cluster_value) >= FAT32_CLUSTER_RESERVED_MIN && (cluster_value) <= FAT32_CLUSTER_RESERVED_MAX)
#define FSC_FAT_CLUSTER_BAD 0xFFFFFF07
#define FSC_FAT_CLUSTER_EOC_MIN 0xFFFFFF08
#define FSC_FAT_CLUSTER_EOC_MAX 0xFFFFFFFF
#define FSC_FAT_CLUSTER_IS_EOC(cluster_value) \
    ((cluster_value) >= FAT32_CLUSTER_EOC_MIN && (cluster_value) <= FAT32_CLUSTER_EOC_MAX)

#define FSC_FAT_DIR_FREE 0x05
#define FSC_FAT_DIR_EOD 0x00 // end of directory

#define FSC_FAT_LFN_LAST_ENTRY_MASK 0x40
#define FSC_FAT_LFN_SEQUENCE_MASK 0x3F

#endif /** __FILEUUID1c3c891456d547aba4b1f1d2e33a11d9 */
