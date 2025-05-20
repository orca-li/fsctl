#include <stdio.h>
#include "public.h"
#include "option.h"
#include "logger.h"
#include "get_info.h"

static int SetupLogger(void)
{
    fscLogger.input = stdin;
    fscLogger.output = stdout;

    return EXIT_SUCCESS;
}

static int SetupInfo(char **argv)
{
    FscGetInfo.program.path = argv[0];

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    SetupLogger();
    SetupInfo(argv);
    FscOption.getting(argc, argv);
    FscOption.call();
    exit(EXIT_SUCCESS);
}
