#include <shell.h>
#include <stdlib.h>


void run_shell()
{

    // Following taken from word example of transitioning states.
    static struct dc_fsm_transition transitions[] = {
            {DC_FSM_INIT,       INIT_STATE,        init_state},
            {INIT_STATE,        READ_COMMANDS,     read_commands},
            {INIT_STATE,        ERROR,             handler_error},
            {READ_COMMANDS,     RESET_STATE,       reset_state},
            {READ_COMMANDS,     SEPARATE_COMMANDS, separate_commands},
            {READ_COMMANDS,     ERROR,             handler_error},
            {SEPARATE_COMMANDS, PARSE_COMMANDS,    parse_commands},
            {SEPARATE_COMMANDS, ERROR,             handler_error},
            {PARSE_COMMANDS,    EXECUTE_COMMANDS,  execute_commands},
            {PARSE_COMMANDS,    ERROR,             handler_error},
            {EXECUTE_COMMANDS,  RESET_STATE,       reset_state},
            {EXECUTE_COMMANDS,  EXIT,              do_exit},
            {EXECUTE_COMMANDS,  ERROR,             handler_error},
            {RESET_STATE,       READ_COMMANDS,     read_commands},
            {EXIT,              DESTROY_STATE,     destroy_state},
            {ERROR,             RESET_STATE,       reset_state},
            {ERROR,             DESTROY_STATE,     destroy_state},
            {DESTROY_STATE,     DC_FSM_EXIT, NULL},
    };

    //
    dc_env_tracer tracer;
    struct dc_error *err;
    struct dc_env *env;
    int from_state, to_state;
    struct dc_fsm_info *fsm_info;
    struct state* currentState = malloc(sizeof(struct state));

    err = dc_error_create(true);
    tracer = dc_env_create(err,false, tracer);

    // this initiates and creates the current fsm.
    fsm_info = dc_fsm_info_create(env, err, "ShellFSM");

    // running of the shell.
    //ar arg is the sate


    dc_fsm_run(env, err, fsm_info, &from_state, &to_state, currentState, transitions);

    // free any memory.
    dc_fsm_info_destroy(env, fsm_info);
    free(currentState->current_line);
    free(currentState->prompt);
    free(currentState->paths);
    free(currentState);

}
