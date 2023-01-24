#ifndef DC_SHELL_BUILTINS_H
#define DC_SHELL_BUILTINS_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <dc_util/path.h>
#include "command.h"

void builtin_cd(const struct dc_env *env, struct dc_error *err, struct state *currentState);

#endif //DC_SHELL_BUILTINS_H
