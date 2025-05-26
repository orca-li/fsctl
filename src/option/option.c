#include <getopt.h>
#include <string.h>
#define FSC_INC_OBJECTS_OPTION_PRIVATE
#define FSC_INC_OBJECTS_VSHELL_PROTECTED
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
#define sub_option_list_s_long 'l'
#define s_option_path 'p'
#define l_option_path "path"
#define s_option_verbose 'V'
#define l_option_verbose "verbose"
#define s_option_flags 'f'
#define l_option_flags "flags"
#define s_option_vshell 'S'
#define l_option_vshell "shell"

static void BindSubFlags(const char *flags);
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
    .flags = {
        .once = false,
        .string = NULL,
        .list = {
            .fmtlong = 0,
            .fmtmax = 0,
        },
    },
    // private
    .set_main_line = SetMainLine,
    .check_main_line = CheckMainLine,
    .bind_sub_flags = BindSubFlags,
};

static void BindSubFlags(const char *flags)
{
    if (fscOption.flags.once)
    {
        fscLogger.warning("There can only be one list of sub-options! The new list under options '%s' is rejected. The current list of sub-options is '%s'.",
                          flags,
                          fscOption.flags.string);
        return;
    }

    fscOption.flags.once = true;
    fscOption.flags.string = flags;
}

static int GetSubFlag(const char *flags)
{
    static bool finish = true;
    static int flag = -1;
    static const char *p = NULL;

    if (finish == true)
    {
        finish = false;
        p = flags;
        flag = *p++;
        if (flag == '\0')
            goto exit_finish;
        return flag;
    }

    flag = *p++;
    if (flag == '\0')
        goto exit_finish;
    return flag;

exit_finish:
    finish = true;
    flag = -1;
    p = NULL;
    return flag;
}

static void SetSubFlagsForMainLine_list(void)
{
    int sub_flag;

    const char *flags = fscOption.flags.string;
    if (flags == NULL)
    {
        fscLogger.error("Error receiving flags: (null)");
        exit(EXIT_FAILURE);
    }

    while ((sub_flag = GetSubFlag(flags)) != -1)
    {
        switch (sub_flag)
        {
        case sub_option_list_s_long:
            fscOption.flags.list.fmtlong = true;
            break;

        default:
            fscLogger.error("Invalid sub flag: %c(%02hhx)", sub_flag, sub_flag);
            fscLogger.info("Usage: %s --help", fscGetInfo.program.path);
            exit(EXIT_FAILURE);
        }
    }
}

static void SetSubFlags(void)
{
    switch (fscOption.check_main_line())
    {
    case s_option_list:
        SetSubFlagsForMainLine_list();
        break;

    default:
        fscLogger.error("The current mainline(%c) does not support sub flags", fscOption.main_line);
        exit(EXIT_FAILURE);
    }
}

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

    fscLogger.error("Select the main line: -h, -v, -l or other options");
    exit(EXIT_FAILURE);
}

static void PrintHelpOption(void)
{
    fscLogger.print("Usage: %s [primary] [-f[flags]] [options]\n", fscGetInfo.program.path);
    fscLogger.print("Primary:\n");
    fscLogger.print("  -%c, --%s\t\t\t\tDisplay this help message and exit.\n", s_option_help, l_option_help);
    fscLogger.print("  -%c, --%s\t\t\t\tDisplay version information and exit.\n", s_option_version, l_option_version);
    fscLogger.print("  -%c [-i<img>], --%s [-i<img>]\tDisplay a list of directories.\n", s_option_list, l_option_list);
    fscLogger.print("  -%c, --%s\t\t\t\tUse a virtual shell\n", s_option_vshell, l_option_vshell);
    fscLogger.print("\n");
    fscLogger.print("Flags:\n");
    fscLogger.print("  --%s -f[flags]\n", l_option_list);
    fscLogger.print("\t-%c\t\t\t\tOutput a long list of files\n", sub_option_list_s_long);
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
        {l_option_flags, required_argument, 0, s_option_flags},
        {l_option_vshell, no_argument, 0, s_option_vshell},
        {0, 0, 0, 0}};

    // setting default settings
    fscVfs.select_fs(fsc_vfs_fat32);

    // setting user settings
    while ((opt = getopt_long(argc, argv, "hvi:lp:Vf:S", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        // [primary]
        case s_option_help:
        case s_option_version:
        case s_option_list:
        case s_option_vshell:
            fscOption.set_main_line(opt);
            break;
        // [options]
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
        case s_option_flags:
            fscOption.bind_sub_flags(optarg);
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

    if (fscOption.flags.once == true)
        SetSubFlags();

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
    case s_option_vshell:
        fscOption.call = fscVShell.entry;
        break;

    default:
        fscLogger.error("Unknown program line: %c", fscOption.main_line);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
