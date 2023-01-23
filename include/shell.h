#ifndef DC_SHELL_SHELL_H
#define DC_SHELL_SHELL_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <dc_fsm/fsm.h>
#include "command.h"
enum shell_states
{
    INIT_STATE = DC_FSM_USER_START,
    READ_COMMANDS,
    ERROR,
    RESET_STATE,
    SEPARATE_COMMANDS,
    PARSE_COMMANDS,
    EXECUTE_COMMANDS,
    EXIT,
    DESTROY_STATE,
};

int run_shell(struct dc_env *env, struct dc_error *err);

int run(const struct dc_env *env, struct dc_error *err, struct command *command, char **path);

#endif //DC_SHELL_SHELL_H
