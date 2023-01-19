

#ifndef OPEN_STATE_H
#define OPEN_STATE_H

#include <dc_fsm/fsm.h>
#include <dc_posix/dc_time.h>

// States to be used for shell.
enum FSMState {
    INIT_STATE = DC_FSM_USER_START, // Initial shell state.
    READ_COMMANDS, // Reading command state.
    SEPARATE_COMMANDS, // Tokenize teh comm.
    PARSE_COMMANDS, // Parse the commands to shell for next action.
    EXECUTE_COMMANDS, // Execute the command that was parsed.
    RESET_STATE, // Clean memory and get ready for reading again.
    ERROR, // Error exit gracefully.
    EXIT, // Exit the program
    DESTROY_STATE // Clean memory before closing program.
};

// example function of functions being used, to be moved later.
// currently are empty and just placeholders.
int init_state();
int read_commands();
int handler_error();
int reset_state();
int separate_commands();
int handler_error();
int parse_commands();
int execute_commands();
int reset_state();
int do_exit();
int destroy_state();



// Following taken from word example of transitioning states.
static struct dc_fsm_transition transitions[] = {
        {DC_FSM_INIT, INIT_STATE, init_state},
        {INIT_STATE, READ_COMMANDS, read_commands},
        {INIT_STATE, ERROR, handler_error},
        {READ_COMMANDS, RESET_STATE, reset_state},
        {READ_COMMANDS, SEPARATE_COMMANDS, separate_commands},
        {READ_COMMANDS, ERROR, handler_error},
        {SEPARATE_COMMANDS, PARSE_COMMANDS, parse_commands},
        {SEPARATE_COMMANDS, ERROR, handler_error},
        {PARSE_COMMANDS, EXECUTE_COMMANDS, execute_commands},
        {PARSE_COMMANDS, ERROR, handler_error},
        {EXECUTE_COMMANDS, RESET_STATE, reset_state},
        {EXECUTE_COMMANDS, EXIT, do_exit},
        {EXECUTE_COMMANDS, ERROR, handler_error},
        {RESET_STATE, READ_COMMANDS, read_commands},
        {EXIT, DESTROY_STATE, destroy_state},
        {ERROR, RESET_STATE, reset_state},
        {ERROR, DESTROY_STATE, destroy_state},
        {DESTROY_STATE,     DC_FSM_EXIT, NULL},
};

#endif //OPEN_STATE_H
