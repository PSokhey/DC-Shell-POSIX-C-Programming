
#ifndef OPEN_UTIL_H
#define OPEN_UTIL_H

#include <regex.h>
#include <stdbool.h>
#include <command.h>

// Current input state of shell.
struct State {
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

#endif //OPEN_UTIL_H
