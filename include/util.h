
#ifndef OPEN_UTIL_H
#define OPEN_UTIL_H

#include <regex.h>
#include <stdbool.h>

// Current input state of shell.
struct state {
    regex_t in_redirect_regex;
    regex_t out_redirect_regex;
    regex_t err_redirect_regex;
    char *paths; //pointer to an array of directories.
    //int path_count;
    char *prompt; // prompt for the user, default $
    int max_line_length;
    char *current_line;
    int current_line_length;
    char *command;
    bool fatal_error;

};

#endif //OPEN_UTIL_H
