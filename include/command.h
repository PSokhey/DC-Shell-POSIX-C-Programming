

#ifndef OPEN_COMMAND_H
#define OPEN_COMMAND_H

#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>

// Pointers might change to char*.
struct command {
    char *line; // The command line for this command
    char* command; // The command (e.g. ls, exit, cd, cat)
    int argc; // The number of arguments passed to the command
    char** argv; // The arguments passed to the command
    int stdin_file; // The file to redirect stdin from, or NULL
    int stdout_file; // The file to redirect stdout to, or NULL
    bool std_overwrite; // Append to or truncate the stdout file
    bool stderr_overwrite; // Append to or truncate the stderr file
    int exit_code; // The status returned from the command
};

static struct command* createCommand();

void destoryCommandStruct(struct command);

#endif //OPEN_COMMAND_H
