#ifndef DC_SHELL_COMMAND_H
#define DC_SHELL_COMMAND_H

#include <stdlib.h>
#include <stdbool.h>
#include <dc_error/error.h>
#include <dc_env/env.h>
#include <dc_posix/dc_wordexp.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "state.h"

struct command {
    char *line;
    char *command;
    size_t argc;
    char **argv;
    char *stdin_file;
    char *stdout_file;
    bool stdout_overwrite;
    char *stderr_file;
    bool stderr_overwrite;
    int exit_code;
};
void redirect(const struct dc_env *env, struct dc_error *err, struct state* currentState);
int parse_command(const struct dc_env *env, struct dc_error *err,
                  struct state *currentState);



#endif //DC_SHELL_COMMAND_H
