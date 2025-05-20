#include "public.h"
#include "logger.h"
#include "get_info.h"

#include <getopt.h>
#include <string.h>

typedef struct FSCTL_OPTION_TYPE
{
    // public
    int (*const getting)(int argc, char **argv);
    void (*call)(void);
    // protected
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

fsctl_option_t FscOption = {
    .getting = GettingOptions,
    .call = NULL,
    .main_line = 0,
    .set_main_line = SetMainLine,
    .check_main_line = CheckMainLine,
};

static void SetMainLine(int line)
{
    if (FscOption.main_line == 0)
    {
        *(int *)&FscOption.main_line = line;
        return;
    }

    fscLogger.warning("There can only be one main line. The new line is ignored. Current line: -%c. The line is rejected: -%c\n", FscOption.main_line, line);
}

static int CheckMainLine(void)
{
    if (FscOption.main_line != 0)
        return FscOption.main_line;

    fscLogger.error("Select the main line: -h, -v or other options\n");
    exit(EXIT_FAILURE);
}

static void PrintHelpOption(void)
{
    printf("Usage: %s [options]\n", FscGetInfo.program.path);
    printf("Options:\n");
    printf("  -h, --help\t\t\t\tDisplay this help message and exit.\n");
    printf("  -v, --version\t\t\t\tDisplay version information and exit.\n");
    printf("\n");
    printf("Example:\n");
    printf("  %s --help\n", FscGetInfo.program.name);
    printf("  %s --version\n", FscGetInfo.program.name);
}

static void PrintVersionOption(void)
{
    printf("=== Version %s ===\nCopyright (C) %d %s. All rights reserved.\n", FscGetInfo.version.string, FscGetInfo.version.year, FscGetInfo.program.vendor);
}

static int GettingOptions(int argc, char **argv)
{
    int opt;
    int option_index = 0;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "hv", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        // Main options
        case 'h':
        case 'v':
            FscOption.set_main_line(opt);
            break;
        case '?':
            fscLogger.error("Invalid option: %c(%02hhx)\n", opt, opt);
            fscLogger.info("Usage: %s --help\n", FscGetInfo.program.path);
            exit(EXIT_FAILURE);

        default:
            fscLogger.error("Unexpected error while processing options\n");
            exit(EXIT_FAILURE);
        }
    }

    switch (FscOption.check_main_line())
    {
    case 'v':
        FscOption.call = PrintVersionOption;
        break;
    case 'h':
        FscOption.call = PrintHelpOption;
        break;

    default:
        fscLogger.error("Unknown program line: %c", FscOption.main_line);
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
