

#ifndef OPEN_COMMAND_H
#define OPEN_COMMAND_H

#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>

// Pointers might change to char*.
struct command {
    char *line; // The command line for this command
    char* command; // The command (e.g. ls, exit, cd, cat)
    size_t argc; // The number of arguments passed to the command
    char** argv; // The arguments passed to the command
    char* stdin_file; // The file to redirect stdin from, or NULL
    char* stdout_file; // The file to redirect stdout to, or NULL
    char* stderror_file;
    bool std_overwrite; // Append to or truncate the stdout file
    bool stderr_overwrite; // Append to or truncate the stderr file
    int exit_code; // The status returned from the command
};



#endif //OPEN_COMMAND_H
