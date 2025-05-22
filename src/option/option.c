#include <getopt.h>
#include <string.h>
#define FSC_INC_OBJECTS_OPTION_PRIVATE
#include "public.h"
#include "objects.h"

#define s_option_help 'h'
#define l_option_help "help"
#define s_option_version 'v'
#define l_option_version "version"
#define s_option_image 'i'
#define l_option_image "image"
#define s_option_list 'l'
#define l_option_list "list"
#define s_option_path 'p'
#define l_option_path "path"
#define s_option_verbose 'V'
#define l_option_verbose "verbose"

static int GettingOptions(int argc, char **argv);
static void PrintHelpOption(void);
static void PrintVersionOption(void);
static void SetMainLine(int line);
static int CheckMainLine(void);

fsctl_option_t fscOption = {
    // public
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
    fscLogger.print("Usage: %s [primary] [-f[flags]] [options]\n", fscGetInfo.program.path);
    fscLogger.print("Primary:\n");
    fscLogger.print("  -%c, --%s\t\t\t\tDisplay this help message and exit.\n", s_option_help, l_option_help);
    fscLogger.print("  -%c, --%s\t\t\t\tDisplay version information and exit.\n", s_option_version, l_option_version);
    fscLogger.print("  -%c [-i<img>], --%s [-i<img>]\tDisplay a list of directories.\n", s_option_list, l_option_list);
    fscLogger.print("\n");
    fscLogger.print("Flags:\n");
    fscLogger.print("\n");
    fscLogger.print("Options:\n");
    fscLogger.print("  -%c, --%s\t\t\t\tSet the path to the file system image.\n", s_option_path, l_option_path);
    fscLogger.print("  -%c, --%s\t\t\t\tEnable verbose output\n", s_option_verbose, l_option_verbose);
    fscLogger.print("\n");
    fscLogger.print("Example:\n");
    fscLogger.print("  %s -%c%c -%c <img>\n", fscGetInfo.program.name, s_option_verbose, s_option_list, s_option_image);
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
        {l_option_version, no_argument, 0, s_option_version},
        {l_option_image, required_argument, 0, s_option_image},
        {l_option_list, no_argument, 0, s_option_list},
        {l_option_path, required_argument, 0, s_option_path},
        {l_option_verbose, no_argument, 0, s_option_verbose},
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
        case s_option_version:
        case s_option_list:
            fscOption.set_main_line(opt);
            break;
        case s_option_image:
            fscOption.input_image = optarg;
            fscVfs.mount();
            break;
        case s_option_path:
            fscVfs.path = optarg;
            break;
        case s_option_verbose:
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

    switch (fscOption.check_main_line())
    {
    case s_option_version:
        fscOption.call = PrintVersionOption;
        break;
    case s_option_help:
        fscOption.call = PrintHelpOption;
        break;
    case s_option_list:
        fscOption.call = fscVfs.list;
        break;

    default:
        fscLogger.error("Unknown program line: %c", fscOption.main_line);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
