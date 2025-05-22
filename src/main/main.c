#include <stdio.h>
#define FSC_INC_OBJECTS_LOGGER_PROTECTED
#include "objects.h"
#include "public.h"


static int SetupLogger(void)
{
    fscLogger.input = stdin;
    fscLogger.output = stdout;

    return EXIT_SUCCESS;
}

static int SetupInfo(char **argv)
{
    fscGetInfo.program.path = argv[0];

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    SetupLogger();
    SetupInfo(argv);
    fscOption.getting(argc, argv);
    fscOption.call();
    exit(EXIT_SUCCESS);
}
