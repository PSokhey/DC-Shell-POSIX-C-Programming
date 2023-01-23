#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dc_env/env.h>
#include <dc_error/error.h>
#include <dc_fsm/fsm.h>
#include <dc_posix/dc_unistd.h>
#include "shell.h"
#include "shell_impl.h"
#include "util.h"

int shell() {
    int ret_val;
    struct dc_fsm_info *fsm_info;
    static struct dc_fsm_transition transitions[] = {
            {DC_FSM_INIT,       INIT_STATE,        init_state},
            {INIT_STATE,        READ_COMMANDS,     read_commands},
            {INIT_STATE,        ERROR,             handle_error},
            {READ_COMMANDS,     RESET_STATE,       reset_state},
            {READ_COMMANDS,     SEPARATE_COMMANDS, separate_commands},
            {READ_COMMANDS,     ERROR,             handle_error},
            {SEPARATE_COMMANDS, PARSE_COMMANDS,    parse_commands},
            {SEPARATE_COMMANDS, ERROR,             handle_error},
            {PARSE_COMMANDS,    EXECUTE_COMMANDS,  execute_commands},
            {PARSE_COMMANDS,    ERROR,             handle_error},
            {EXECUTE_COMMANDS,  RESET_STATE,       reset_state},
            {EXECUTE_COMMANDS,  EXIT,              do_exit},
            {EXECUTE_COMMANDS,  ERROR,             handle_error},
            {RESET_STATE,       READ_COMMANDS,     read_commands},
            {EXIT,              DESTROY_STATE,     destroy_state},
            {ERROR,             RESET_STATE,       reset_state},
            {ERROR,             DESTROY_STATE,     destroy_state},
            {DESTROY_STATE,     DC_FSM_EXIT, NULL},
    };

    dc_env_tracer tracer; // To trace the program.
    struct dc_env *env; // For error handling.
    struct dc_error *err; // environment of the program.

    //int return_val = 0;

    // Turn the tracer (tracing the program) on or off.
    tracer = dc_env_default_tracer;
    //tracer = NULL;

    err = dc_error_create(false);
    env = dc_env_create(err, false, tracer);
    dc_env_set_tracer(env, tracer);
    //dc_error_init(err, false);

    //dc_error_init(err, false);
    dc_env_set_tracer(env, NULL);
    ret_val = EXIT_SUCCESS;
    fsm_info = dc_fsm_info_create(env, err, "shell");
    if (dc_error_has_no_error(err)) {
        int from_state, to_state;
        struct state state;
        ret_val = dc_fsm_run(env, err, fsm_info, &from_state,
                             &to_state, &state, transitions);
        dc_fsm_info_destroy(env, &fsm_info);
    }
    return ret_val;
}

// P: for running the command. no in main, thats all above.
int run(const struct dc_env *env, struct dc_error *err, struct command *command, char **path) {

    if (strstr(command->command, "/") != NULL) {
        command->argv[0] = command->command;
        dc_execve(env, err, command->command, command->argv, NULL);
    } else {
        if (path[0] == NULL) {
            DC_ERROR_RAISE_CHECK(err);
            fprintf(stderr, "Error: %s\n", strerror(ENOENT));
        } else {
            for (char * new_com = *path; new_com; new_com = *path++) {
                //printf("%s\n", new_com);
                char * dest = my_strcat(new_com, "/");
                dest = my_strcat(dest, command->command);
                command->argv[0] = dest;
                dc_execvp(env, err, dest, command->argv);
                if (dc_error_has_error(err)){
                    if (!dc_error_is_errno(err, ENOENT))
                        break;
                }
            }
        }
    }
}
