#include <dc_error/error.h>
#include <dc_env/env.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "state.h"
#include "command.h"
#include "builtins.h"
#include "shell.h"
#include "execute.h"
#include "util.h"
#include "shell_impl.h"
#include <dc_fsm/fsm.h>
#include <dc_posix/dc_stdio.h>
#include <dc_util/strings.h>
#include <dc_util/filesystem.h>

typedef struct command command;
regex_t err_regex;
regex_t in_regex;
regex_t out_regex;

int init_state(const struct dc_env *env, struct dc_error *err, struct state* currentState) {

    memset(currentState, 0, sizeof(struct state));
    currentState->fatal_error = false;
    currentState->max_line_length = sysconf(_SC_ARG_MAX);

    regcomp(&in_regex, "[ \t\f\v]<.*", REG_EXTENDED);
    regcomp(&out_regex, "[ \t\f\v][1^2]?>[>]?.*", REG_EXTENDED);
    regcomp(&err_regex, "[ \t\f\v]2>[>]?.*", REG_EXTENDED);

    currentState->in_redirect_regex = &in_regex;
    currentState->out_redirect_regex = &out_regex;
    currentState->err_redirect_regex = &err_regex;
    currentState->path = get_path(env, err, currentState);
    get_prompt(env, err, currentState);
    currentState->command = (struct command *) calloc(1,sizeof(struct command));
    currentState->command->line = NULL;

    if (err != NULL && dc_error_has_error(err)) {
        currentState->fatal_error = true;
        printf("DC HAS ERROR IN MAIN IF STATEMENT");
        return EXIT_FAILURE;
    }
    return READ_COMMANDS;
}


int read_commands(const struct dc_env *env, struct dc_error *err, struct state* currentState) {
    currentState->fatal_error = 0;
    size_t line_len = 0;

    char *cur_dir = dc_get_working_dir(env, err);
    if (dc_error_has_error(err)) {
        currentState->fatal_error = true;
        return ERROR;
    }

    //p: this is dislplaying directory to user.
    fprintf(stdout, "[%s] %s", cur_dir, currentState->prompt);
    currentState->current_line = malloc(sizeof(char));

    if (dc_error_has_error(err)) {
        currentState->fatal_error = true;
    }

    dc_getline(env, err, &currentState->current_line, &line_len, stdin);
    if (dc_error_has_error(err)) {
        currentState->fatal_error = true;
        return ERROR;
    }

    dc_str_trim(env, currentState->current_line);
    printf("Command: %s\n", currentState->current_line);
    line_len = strlen(currentState->current_line);

    if (line_len == 0) {
        return RESET_STATE;
    }

    currentState->current_line_length = line_len;

    return SEPARATE_COMMANDS;
}

int separate_commands(const struct dc_env *env, struct dc_error *err, struct state* currentState) {

    if (dc_error_has_error(err)) {
        currentState->fatal_error = true;
        return ERROR;
    }

    currentState->command = calloc(1, sizeof(command));


    if (currentState->command == NULL) {
        return ERROR;
    }

    currentState->command->line = strdup(currentState->current_line);

    if (currentState->command->line == NULL) {
        free(currentState->command);
        return ERROR;
    }

    currentState->command->command = NULL;
    currentState->command->exit_code = 0;
    currentState->command->argv = NULL;
    currentState->command->argc = 0;
    currentState->command->stdin_file = NULL;
    currentState->command->stdout_file = NULL;
    currentState->command->stdout_overwrite = false;
    currentState->command->stderr_file = NULL;

    return PARSE_COMMANDS;
}

int parse_commands(const struct dc_env *env, struct dc_error *err, struct state* currentState) {
    currentState->fatal_error = 0;
    parse_command(env, err, currentState);
    if (dc_error_has_error(err)) {
        currentState->fatal_error = 1;
        return ERROR;
    }
    return EXECUTE_COMMANDS;
}

int execute_commands(const struct dc_env *env,
                     struct dc_error *err, struct state* currentState) {

    if (strcmp(currentState->command->command, "cd") == 0) {
        builtin_cd(env, err, currentState);
    } else if(strcmp(currentState->command->command, "cd .") == 0){
        builtin_cd(env, err, currentState);
    } else if(strcmp(currentState->command->command, "cd /") == 0){
        builtin_cd(env, err, currentState);
    } else if(strcmp(currentState->command->command, "cd ~") == 0){
        builtin_cd(env, err, currentState);
    } else if(strcmp(currentState->command->command, "cd ..") == 0){
        builtin_cd(env, err, currentState);
    }
    else if (strcmp(currentState->command->command, "exit") == 0) {
        return EXIT;
    } else {
        execute(env, err, currentState, currentState->path);
        if (dc_error_has_error(err)) {
            currentState->fatal_error = true;
        }
    }
    printf("\nExit code: %d\n", currentState->command->exit_code);
    if (currentState->fatal_error) {
        return ERROR;
    }
    return RESET_STATE;
}
int do_exit(const struct dc_env *env, struct dc_error *err, struct state* currentState) {
    do_reset_state(env, err, currentState);
    return DESTROY_STATE;
}

int reset_state(const struct dc_env *env, struct dc_error *error, struct state* currentState) {
    do_reset_state(env, error, currentState);
    return READ_COMMANDS;
}

int handle_error(const struct dc_env *env, struct dc_error *err, struct state* currentState) {

    if (currentState->current_line == NULL) {
        printf("Internal error (%d)\n", currentState->command->exit_code);
    } else {
        printf("Internal error (%d) %s: %s\n",
               currentState->command->exit_code, currentState->command->command, currentState->current_line);
    }
    if (currentState->fatal_error) {
        return DESTROY_STATE;
    }
    return RESET_STATE;
}

// Displaying the appropriate error essage.
int handle_run_error(const struct dc_env *env, struct dc_error *err, struct state* currentState) {

    char* test = "worked";

    if (dc_error_is_errno(err, E2BIG)) {
        fprintf(stdout, "Argument list too long (POSIX.1-2001)\n");
        return 1;
    } else if (dc_error_is_errno(err, EACCES)) {
        fprintf(stdout, "Permission denied (POSIX.1-2001)\n");
        return 2;
    } else if (dc_error_is_errno(err, EINVAL)) {
        fprintf(stdout, "Invalid argument (POSIX.1-2001)\n");
        return 3;
    } else if (dc_error_is_errno(err, ELOOP)) {
        fprintf(stdout, "Too many levels of symbolic links (POSIX.1-2001)\n");
        return 4;
    } else if (dc_error_is_errno(err, ENAMETOOLONG)) {
        fprintf(stdout, "Filename too long (POSIX.1-2001)\n");
        return 5;
    } else if (dc_error_is_errno(err, ENOENT)) {
        fprintf(stdout, "No such file or directory (POSIX.1-2001)\n");
        return 127;
    } else if (dc_error_is_errno(err, ENOTDIR)) {
        fprintf(stdout, "Not a directory (POSIX.1-2001)\n");
        return 6;
    } else if (dc_error_is_errno(err, ENOEXEC)) {
        fprintf(stdout, "Exec format error (POSIX.1-2001)\n");
        return 7;
    } else if (dc_error_is_errno(err, ENOMEM)) {
        fprintf(stdout, "Not enough space/cannot allocate memory (POSIX.1-2001)\n");
        return 8;
    } else if (dc_error_is_errno(err, ETXTBSY)) {
        fprintf(stdout, "Text file busy (POSIX.1-2001)\n");
        return 9;
    } else {
        return 125;
    }
}

int destroy_state(const struct dc_env *env, struct dc_error *err, struct state* currentState) {
    reset_state(env, err, currentState);
    return DC_FSM_EXIT;
}
