#ifndef DC_SHELL_STATE_H
#define DC_SHELL_STATE_H

#include <stdbool.h>
#include <regex.h>
#include "command.h"

//struct command;

// State struct for keeping the state of the program.
struct state {
    regex_t *in_redirect_regex;
    regex_t *out_redirect_regex;
    regex_t *err_redirect_regex;
    char **path;
    char *prompt;
    ssize_t max_line_length;
    char *current_line;
    ssize_t current_line_length;
    struct command *command;
    bool fatal_error;
};
#endif //DC_SHELL_STATE_H
