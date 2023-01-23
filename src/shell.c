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


    int ret_val; // For return code.
    struct dc_fsm_info *fsm_info; // Struct for running the Finite State Machine.
    dc_env_tracer tracer; // To trace the program.
    struct dc_env *env; // For error handling.
    struct dc_error *err; // environment of the program.

    // setting to trace the program.
    tracer = dc_env_default_tracer;
    //tracer = NULL;

    // initiating the error, environment, and tracing variables.
    err = dc_error_create(false);
    env = dc_env_create(err, false, tracer);

    // Set the tracer to dc_env.
    //dc_env_set_tracer(env, tracer);
    dc_env_set_tracer(env, NULL);

    // set struct to run the FSM and the state struct to run program.
    ret_val = EXIT_SUCCESS;
    fsm_info = dc_fsm_info_create(env, err, "Running Shell");
    if (dc_error_has_no_error(err)) {
        int from_state, to_state;
        struct state* currentState = calloc(1,sizeof(struct state));
        memset(currentState, 0, sizeof(struct state));
        ret_val = dc_fsm_run(env, err, fsm_info, &from_state,&to_state, currentState, transitions);
        dc_fsm_info_destroy(env, &fsm_info);
    }
    return ret_val;
}


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
