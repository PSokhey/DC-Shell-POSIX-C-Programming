#ifndef DC_SHELL_SHELL_H
#define DC_SHELL_SHELL_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <dc_fsm/fsm.h>
#include "command.h"
#include <stddef.h>
#include <stdlib.h>

// States defined for the Finite State Machine.
enum FSMState
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

// Called to run the shell.
int shell();

// for running a command.
int run(const struct dc_env *env, struct dc_error *err, struct command *command, char **path);

#endif //DC_SHELL_SHELL_H
