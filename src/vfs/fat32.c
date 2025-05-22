#include "fs/fat.h"
#include "logger.h"
#include "fscio.h"
#include "protected/option.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef struct FSCTL_FAT32_HANDLER_TYPE
{
    // public
    void (*mount)(void);
    void (*list)(const char *path);
    void (*umount)(void);
    // protected
    // private
} fsc_fat32_handler_t;

static void HandleMount(void);
static void HandleList(const char *path);
static void HandleUmount(void);

fsc_fat32_handler_t fscFat32Handler = {
    // public
    .mount = HandleMount,
    .list = HandleList,
    .umount = HandleUmount,
    // protected
    // private
};

static void HandleMount(void)
{
    fscIO.image.read(fscOption.input_image);

    if (fscIO.image.handler == NULL)
    {
        fscLogger.error("The FAT32 file system was not mounted");
        exit(EXIT_FAILURE);
    }

    fscFat32BootSector_t boot_sector;
    if (fread(&boot_sector, sizeof(fscFat32BootSector_t), 1, fscIO.image.handler) != 1)
    {
        fscLogger.error("Error reading boot sector");
        fscIO.image.close();
        exit(EXIT_FAILURE);
    }

    fscLogger.verbose("Mounted FAT32");
    fscLogger.verbose("Bytes per sector: %d", boot_sector.BytsPerSec);
    fscLogger.verbose("Sectors per cluster: %d", boot_sector.SecPerClus);
    fscLogger.verbose("Number of FATs: %d", boot_sector.NumFATs);
    fscLogger.verbose("FAT size (sectors): %d", boot_sector.FATSz32);
    fscLogger.verbose("Root cluster: %d", boot_sector.RootClus);

    uint32_t root_cluster = boot_sector.RootClus;
    uint32_t bytes_per_sector = boot_sector.BytsPerSec;
    uint32_t sectors_per_cluster = boot_sector.SecPerClus;
    uint32_t reserved_sectors = boot_sector.RsvdSecCnt;
    uint32_t fat_size_sectors = boot_sector.FATSz32;
    uint32_t num_fats = boot_sector.NumFATs;

    uint32_t root_dir_sector = reserved_sectors + (num_fats * fat_size_sectors) + ((root_cluster - 2) * sectors_per_cluster);
    uint32_t root_dir_offset = root_dir_sector * bytes_per_sector;

    fscLogger.verbose("Root dir offset: %x", root_dir_offset);

    fseek(fscIO.image.handler, root_dir_offset, SEEK_SET);
}

static void HandleUmount(void)
{
    fscIO.image.close();
    fscLogger.verbose("Unmounted FAT32");
}

static const char *FormatAttributes(uint8_t attr)
{
    static char attrs[6] = "-----";
    if (attr & FSC_FAT_ATTR_DIRECTORY)
        attrs[4] = 'D';
    if (attr & FSC_FAT_ATTR_ARCHIVE)
        attrs[3] = 'A';
    if (attr & FSC_FAT_ATTR_SYSTEM)
        attrs[2] = 'S';
    if (attr & FSC_FAT_ATTR_HIDDEN)
        attrs[1] = 'H';
    if (attr & FSC_FAT_ATTR_READ_ONLY)
        attrs[0] = 'R';
    return attrs;
}

static void FormatDateTime(uint16_t date, uint16_t time, char *buffer, size_t size)
{
    snprintf(buffer, size, "%04u-%02u-%02u %02u:%02u",
             1980 + (date >> 9),
             (date >> 5) & 0x0F,
             date & 0x1F,
             time >> 11,
             (time >> 5) & 0x3F);
}

static void FormatShortName(const uint8_t fat_name[11], char *output)
{
    int pos = 0;
    for (int i = 0; i < 8 && fat_name[i] != ' '; i++)
    {
        output[pos++] = fat_name[i];
    }

    if (fat_name[8] != ' ')
    {
        output[pos++] = '.';
        for (int i = 8; i < 11 && fat_name[i] != ' '; i++)
        {
            output[pos++] = fat_name[i];
        }
    }
    output[pos] = '\0';
}

static bool BuildLFNName(fscFatLongDir_t *lfn_chain, int count, const uint8_t *short_name, char *output)
{
    uint8_t checksum = lfn_chain[0].Chksum;
    int pos = 0;

    uint8_t sum = 0;
    for (int i = 0; i < 11; i++)
    {
        sum = ((sum & 1) ? 0x80 : 0) + (sum >> 1) + short_name[i];
    }
    if (sum != checksum)
        return false;

    for (int i = count - 1; i >= 0; i--)
    {
        const fscFatLongDir_t *entry = &lfn_chain[i];

        for (int j = 0; j < 10; j += 2)
        {
            if (entry->Name1[j] == 0 && entry->Name1[j + 1] == 0)
                break;
            output[pos++] = entry->Name1[j];
        }
        for (int j = 0; j < 12; j += 2)
        {
            if (entry->Name2[j] == 0 && entry->Name2[j + 1] == 0)
                break;
            output[pos++] = entry->Name2[j];
        }
        for (int j = 0; j < 4; j += 2)
        {
            if (entry->Name3[j] == 0 && entry->Name3[j + 1] == 0)
                break;
            output[pos++] = entry->Name3[j];
        }
    }
    output[pos] = '\0';
    return true;
}

static void HandleList(const char *path)
{
    (void)path;
    fscFatDir_t dir_entry;
    fscFatLongDir_t lfn_chain[20];
    int lfn_count = 0;
    char display_name[256];
    char datetime[20];

    while (fread(&dir_entry, sizeof(dir_entry), 1, fscIO.image.handler) == 1)
    {
        if (dir_entry.Name[0] == FSC_FAT_DIR_EOD)
            break;

        if (dir_entry.Name[0] == FSC_FAT_DIR_FREE || dir_entry.Name[0] == 0xE5)
            continue;

        if (dir_entry.Attr == FSC_FAT_ATTR_LONG_NAME)
        {
            if (lfn_count < 20)
            {
                memcpy(&lfn_chain[lfn_count++], &dir_entry, sizeof(fscFatLongDir_t));
            }
            continue;
        }

        if (lfn_count > 0 && BuildLFNName(lfn_chain, lfn_count, dir_entry.Name, display_name))
        {
        }
        else
        {
            FormatShortName(dir_entry.Name, display_name);
        }
        lfn_count = 0;

        if (display_name[0] == '.')
            continue;

        FormatDateTime(dir_entry.WrtDate, dir_entry.WrtTime, datetime, sizeof(datetime));

        fscLogger.print("%5u %5s %s %-15s\n",
                        dir_entry.FileSize,
                        FormatAttributes(dir_entry.Attr),
                        datetime,
                        display_name);
    }
}

bool VerifyChecksum(const char *shortName, uint8_t checksum)
{
    uint8_t sum = 0;
    for (int i = 0; i < 11; i++)
    {
        sum = ((sum & 1) << 7) + (sum >> 1) + (unsigned char)shortName[i];
    }
    return (sum == checksum);
}