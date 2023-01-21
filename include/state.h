

#ifndef OPEN_STATE_H
#define OPEN_STATE_H

#include <dc_fsm/fsm.h>
#include <regex.h>
#include <unistd.h>
#include <stdlib.h>



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

};

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




#endif //OPEN_STATE_H
