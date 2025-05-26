#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define FSC_INC_OBJECTS_VSHELL_PRIVATE
#include "objects.h"
#include "public.h"

#define VSHELL_PROMPT "vsh > "
#define VSHELL_BAD_PROMPT "vsh[%d] > "
#define MAX_ARGS 10
#define MAX_ARG_LENGTH 64
#define INPUT_BUFFER_SIZE 256

typedef enum command_list_enum
{
    command_ls,
    command_exit,
    command_skip,
    command_unknown
} command_list_e;

typedef struct
{
    int argc;
    char argv[MAX_ARGS][MAX_ARG_LENGTH];
} command_args_t;

static void PrintPrompt(void);
static void VShellEntry(void);

static char PrintWorkingDirectory[1024] = "/";
fsc_vshell_t fscVShell = {
    // public
    .entry = VShellEntry,
    // protected
    .pwd = PrintWorkingDirectory,
    // private
    .retval = EXIT_SUCCESS,
};

static command_list_e CommandSearch(const char *command)
{
    while (*command == ' ' || *command == '\t')
        command++;

    if (*command == '\0')
        return command_skip;

    if (strncmp(command, "exit", 4) == 0 &&
        (command[4] == ' ' || command[4] == '\t' || command[4] == '\0'))
        return command_exit;

    if (strncmp(command, "ls", 2) == 0 &&
        (command[2] == ' ' || command[2] == '\t' || command[2] == '\0'))
        return command_ls;

    return command_unknown;
}

static void ParseCommandArgs(const char *input, command_args_t *args)
{
    args->argc = 0;
    char temp[INPUT_BUFFER_SIZE];
    size_t input_len = strnlen(input, INPUT_BUFFER_SIZE - 1);
    strncpy(temp, input, input_len);
    temp[input_len] = '\0';

    char *token = strtok(temp, " \t");
    while (token != NULL && args->argc < MAX_ARGS)
    {
        strncpy(args->argv[args->argc], token, MAX_ARG_LENGTH - 1);
        args->argv[args->argc][MAX_ARG_LENGTH - 1] = '\0';
        args->argc++;
        token = strtok(NULL, " \t");
    }
}

static command_list_e GetCommand(command_args_t *args)
{
    char buffer[INPUT_BUFFER_SIZE];

    PrintPrompt();

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return command_exit;

    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[0] == '\0')
        return command_skip;

    ParseCommandArgs(buffer, args);

    if (args->argc == 0)
        return command_skip;

    return CommandSearch(args->argv[0]);
}

static void ExecuteCommand(command_list_e command, const command_args_t *args)
{
    switch (command)
    {
    case command_ls:
        if (fscVfs.list)
        {
            fscVfs.list();
            fscVShell.retval = EXIT_SUCCESS;
        }
        else
        {
            fscLogger.error("vshell: ls command not implemented");
            fscVShell.retval = EXIT_FAILURE;
        }
        break;

    case command_unknown:
        if (args->argc > 0)
            printf("Unknown command: %s\n", args->argv[0]);
        fscVShell.retval = EXIT_FAILURE;
        break;

    case command_skip:
        break;

    default:
        fscLogger.error("vshell: ExecuteCommand(undefined)");
        break;
    }
}

static void PrintPrompt(void)
{
    printf((fscVShell.retval == 0) ? VSHELL_PROMPT : VSHELL_BAD_PROMPT, fscVShell.retval);
    fflush(stdout);
}

static void VShellEntry(void)
{
    command_args_t args;
    command_list_e command;

    while (1)
    {
        command = GetCommand(&args);

        if (command == command_exit)
            break;

        ExecuteCommand(command, &args);
    }
}