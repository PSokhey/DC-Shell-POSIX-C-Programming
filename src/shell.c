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
    memset(currentState, 0, sizeof(struct state));
    //struct state currentState;
    err = dc_error_create(true);
    tracer = NULL;
    env = dc_env_create(err,false, tracer);

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

static struct state* create_state() {

    // Allocate memory for struct.
    struct state* new_state = malloc(sizeof(struct state));
    if (!new_state) {
        // handle allocation error
        return NULL;
    }

    // initialize all variables
    new_state->paths = NULL;
    new_state->prompt = NULL;
    new_state->current_line = NULL;
    new_state->command = NULL;
    new_state->fatal_error = false;
    new_state->max_line_length = 0;
    new_state->current_line_length = 0;

    // compile regular expressions
    int ret_code = regcomp(&new_state->in_redirect_regex, "[ \t\f\v]<.*", REG_EXTENDED);
    if (ret_code != 0) {
        // handle regular expression compilation error
        free(new_state);
        return NULL;
    }
    ret_code = regcomp(&new_state->out_redirect_regex, "[ \t\f\v][1^2]?>[>]?.*", REG_EXTENDED);
    if (ret_code != 0) {
        // handle regular expression compilation error
        regfree(&new_state->in_redirect_regex);
        free(new_state);
        return NULL;
    }
    ret_code = regcomp(&new_state->err_redirect_regex, "[ \t\f\v]2>[>]?.*", REG_EXTENDED);
    if (ret_code != 0) {
        // handle regular expression compilation error
        regfree(&new_state->in_redirect_regex);
        regfree(&new_state->out_redirect_regex);
        free(new_state);
        return NULL;
    }

    return new_state;
}
