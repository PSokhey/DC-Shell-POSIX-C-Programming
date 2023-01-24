#ifndef DC_SHELL_EXECUTE_H
#define DC_SHELL_EXECUTE_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <sys/wait.h>
#include <dc_posix/dc_unistd.h>
#include "command.h"

/**
 * Execute command.
 * @param env pointer to enviromnent object.
 * @param err pointer to error object.
 * @param currentState pointer to the state object.
 * @param path array for the paths.
 */
void execute(const struct dc_env *env, struct dc_error *err, struct state* currentState, char ** path);

#endif //DC_SHELL_EXECUTE_H
