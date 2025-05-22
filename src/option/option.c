#include "public.h"
#include "logger.h"
#include "get_info.h"
#include "public/vfs.h"

#include <getopt.h>
#include <string.h>

#define s_option_help 'h'
#define l_option_help "help"


typedef struct FSCTL_OPTION_TYPE
{
    // public
    int (*const getting)(int argc, char **argv);
    void (*call)(void);
    // protected
    const char *input_image;
    volatile int const main_line;
    // private
    void (*const set_main_line)(int line);
    int (*const check_main_line)(void);
} fsctl_option_t;

static int GettingOptions(int argc, char **argv);
static void PrintHelpOption(void);
static void PrintVersionOption(void);
static void SetMainLine(int line);
static int CheckMainLine(void);

fsctl_option_t fscOption = {
    // pbulic
    .getting = GettingOptions,
    .call = NULL,
    // protected
    .input_image = NULL,
    .main_line = 0,
    // private
    .set_main_line = SetMainLine,
    .check_main_line = CheckMainLine,
};



static void SetMainLine(int line)
{
    if (fscOption.main_line == 0)
    {
        *(int *)&fscOption.main_line = line;
        return;
    }

    fscLogger.warning("There can only be one main line. The new line is ignored. Current line: -%c. The line is rejected: -%c", fscOption.main_line, line);
}

static int CheckMainLine(void)
{
    if (fscOption.main_line != 0)
        return fscOption.main_line;

    fscLogger.error("Select the main line: -h, -v or other options");
    exit(EXIT_FAILURE);
}

static void PrintHelpOption(void)
{
    fscLogger.print("Usage: %s [options]\n", fscGetInfo.program.path);
    fscLogger.print("Options:\n");
    fscLogger.print("  -h, --help\t\t\t\tDisplay this help message and exit.\n");
    fscLogger.print("  -v, --version\t\t\t\tDisplay version information and exit.\n");
    fscLogger.print("  -i, --image\t\t\t\tSet the path to the file system image.\n");
    fscLogger.print("\n");
    fscLogger.print("Example:\n");
    fscLogger.print("  %s --help\n", fscGetInfo.program.name);
    fscLogger.print("  %s --version\n", fscGetInfo.program.name);
}

static void PrintVersionOption(void)
{
    fscLogger.print("=== Version %s ===\nCopyright (C) %d %s. All rights reserved.\n", fscGetInfo.version.string, fscGetInfo.version.year, fscGetInfo.program.vendor);
}

static int GettingOptions(int argc, char **argv)
{
    int opt;
    int option_index = 0;

    static struct option long_options[] = {
        {l_option_help, no_argument, 0, s_option_help},
        {"version", no_argument, 0, 'v'},
        {"image", required_argument, 0, 'i'},
        {"list", no_argument, 0, 'l'},
        {"path", required_argument, 0, 'p'},
        {"verbose", no_argument, 0, 'V'},
        {0, 0, 0, 0}};

    // setting default settings
    fscVfs.select_fs(fsc_vfs_fat32);

    // setting user settings
    while ((opt = getopt_long(argc, argv, "hvi:lp:V", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        // Main options
        case s_option_help:
        case 'v':
        case 'l':
            fscOption.set_main_line(opt);
            break;
        case 'i':
            fscOption.input_image = optarg;
            fscVfs.mount();
            break;
        case 'p':
            fscVfs.path = optarg;
            break;
        case 'V':
            fscLogger.flags.verbose = true;
            break;
        case '?':
            fscLogger.error("Invalid option: %c(%02hhx)", opt, opt);
            fscLogger.info("Usage: %s --help", fscGetInfo.program.path);
            exit(EXIT_FAILURE);

        default:
            fscLogger.error("Unexpected error while processing options");
            exit(EXIT_FAILURE);
        }
    }

    // setting new options

    switch (fscOption.check_main_line())
    {
    case 'v':
        fscOption.call = PrintVersionOption;
        break;
    case 'h':
        fscOption.call = PrintHelpOption;
        break;
    case 'l':
        fscOption.call = fscVfs.list;
        break;

    default:
        fscLogger.error("Unknown program line: %c", fscOption.main_line);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
