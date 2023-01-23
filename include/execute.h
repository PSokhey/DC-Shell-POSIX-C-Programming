#ifndef DC_SHELL_EXECUTE_H
#define DC_SHELL_EXECUTE_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <dc_posix/dc_unistd.h>
#include <sys/wait.h>
#include "command.h"

void execute(const struct dc_env *env, struct dc_error *err, struct state * state, char ** path);

#endif //DC_SHELL_EXECUTE_H
