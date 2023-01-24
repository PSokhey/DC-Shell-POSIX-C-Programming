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

/**
 * Command struct to contain info for the current command.
 */
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

/**
 * TO redirect stdin and stdout.
 * @param env pointer to environment object.
 * @param err pointer to error object.
 * @param currentState pointer to the current state object.
 */
void redirect(const struct dc_env *env, struct dc_error *err, struct state* currentState);

/**
 * To parse a command to be excecuted.
 * @param env  pointer to the environment object.
 * @param err pointer to the error object.
 * @param currentState pointer to the active state object.
 * @return The next command state.
 */
int parse_command(const struct dc_env *env, struct dc_error *err,
                  struct state *currentState);



#endif //DC_SHELL_COMMAND_H
