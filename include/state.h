

#ifndef OPEN_STATE_H
#define OPEN_STATE_H

#include <dc_fsm/fsm.h>
#include <regex.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>



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

// Current input state of shell.
struct state {
    regex_t in_redirect_regex; // Find input redirection: < path
    regex_t out_redirect_regex; // Find output redirection: [1]>[>] path
    regex_t err_redirect_regex; // Find output redirection: 2>[>] path
    char **paths; //pointer to an array of directories. // An array of directories to search for external commands
    char *prompt; // prompt for the user, default $
    size_t max_line_length; // The longest possible command line
    char *current_line; // The current command line
    size_t current_line_length; // The length of the current command line
    struct Command *command; // The command to execute
    bool fatal_error; // True if an error happened that should exit the shell
    FILE *stdin;
    FILE *stdout;
    FILE *sterr;

};






#endif //OPEN_STATE_H
