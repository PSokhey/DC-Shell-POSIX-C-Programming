#include <dc_error/error.h>
#include <dc_env/env.h>
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

int init_state(const struct dc_env *env, struct dc_error *err, void *arg) {

    struct state *state = (struct state *) arg;
    memset(state, 0, sizeof(struct state));
    state->fatal_error = false;
    state->max_line_length = sysconf(_SC_ARG_MAX);

    regcomp(&in_regex, "[ \t\f\v]<.*", REG_EXTENDED);
    regcomp(&out_regex, "[ \t\f\v][1^2]?>[>]?.*", REG_EXTENDED);
    regcomp(&err_regex, "[ \t\f\v]2>[>]?.*", REG_EXTENDED);

    state->in_redirect_regex = &in_regex;
    state->out_redirect_regex = &out_regex;
    state->err_redirect_regex = &err_regex;
    state->path = get_path(env, err, state);
    get_prompt(env, err, state);
    state->command = (struct command *) malloc(sizeof(struct command));
    state->command->line = NULL;

    if (err != NULL && dc_error_has_error(err)) {
        state->fatal_error = true;
        printf("DC HAS ERROR IN MAIN IF STATEMENT");
        return EXIT_FAILURE;
    }
    return READ_COMMANDS;
}


int read_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    struct state *state = (struct state *) arg;
    state->fatal_error = 0;
    size_t line_len = 0;

    char *cur_dir = dc_get_working_dir(env, err);
    if (dc_error_has_error(err)) {
        state->fatal_error = true;
        return ERROR;
    }

    //p: this is dislplaying directory to user.
    fprintf(stdout, "[%s] %s", cur_dir, state->prompt);

    state->current_line = malloc(sizeof(char));
    if (dc_error_has_error(err)) {
        state->fatal_error = true;
    }

    dc_getline(env, err, &state->current_line, &line_len, stdin);
    if (dc_error_has_error(err)) {
        state->fatal_error = true;
        return ERROR;
    }

    dc_str_trim(env, state->current_line);
    printf("Command: %s\n", state->current_line);
    line_len = strlen(state->current_line);

    if (line_len == 0) {
        return RESET_STATE;
    }

    state->current_line_length = line_len;

    return SEPARATE_COMMANDS;
}

int separate_commands(const struct dc_env *env, struct dc_error *err, void *arg) {

    struct state *state = arg;

    if (dc_error_has_error(err)) {
        state->fatal_error = true;
        return ERROR;
    }

    state->command = calloc(1, sizeof(command));

    if (state->command == NULL) {
        return ERROR;
    }

    state->command->line = strdup(state->current_line);

    if (state->command->line == NULL) {
        free(state->command);
        return ERROR;
    }

    state->command->command = NULL;
    state->command->exit_code = 0;
    state->command->argv = NULL;
    state->command->argc = 0;
    state->command->stdin_file = NULL;
    state->command->stdout_file = NULL;
    state->command->stdout_overwrite = false;
    state->command->stderr_file = NULL;

    return PARSE_COMMANDS;
}

int parse_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    struct state *state = arg;
    state->fatal_error = 0;
    parse_command(env, err, state);
    if (dc_error_has_error(err)) {
        state->fatal_error = 1;
        return ERROR;
    }
    return EXECUTE_COMMANDS;
}

int execute_commands(const struct dc_env *env,
                     struct dc_error *err, void *arg) {

    struct state *state = (struct state *) arg;

    if (strcmp(state->command->command, "cd") == 0) {
        builtin_cd(env, err, state);
    } else if(strcmp(state->command->command, "cd .") == 0){
        builtin_cd(env, err, state);
    } else if(strcmp(state->command->command, "cd /") == 0){
        builtin_cd(env, err, state);
    } else if(strcmp(state->command->command, "cd ~") == 0){
        builtin_cd(env, err, state);
    } else if(strcmp(state->command->command, "cd ..") == 0){
        builtin_cd(env, err, state);
    }
    else if (strcmp(state->command->command, "exit") == 0) {
        return EXIT;
    } else {
        execute(env, err, state, state->path);
        if (dc_error_has_error(err)) {
            state->fatal_error = true;
        }
    }
    printf("\nExit code: %d\n", state->command->exit_code);
    if (state->fatal_error) {
        return ERROR;
    }
    return RESET_STATE;
}
int do_exit(const struct dc_env *env, struct dc_error *err, void *arg) {
    struct state *state = arg;
    do_reset_state(env, err, state);
    return DESTROY_STATE;
}

int reset_state(const struct dc_env *env, struct dc_error *error, void *arg) {
    struct state *state = arg;
    do_reset_state(env, error, state);
    return READ_COMMANDS;
}

int handle_error(const struct dc_env *env, struct dc_error *err, void *arg) {
    struct state *state = arg;

    if (state->current_line == NULL) {
        printf("Internal error (%d)\n", state->command->exit_code);
    } else {
        printf("Internal error (%d) %s: %s\n",
               state->command->exit_code, state->command->command, state->current_line);
    }
    if (state->fatal_error) {
        return DESTROY_STATE;
    }
    return RESET_STATE;
}

int handle_run_error(const struct dc_env *env, struct dc_error *err, void *arg) {
    struct state *state = (struct state *) arg;
    if (dc_error_is_errno(err, E2BIG)) {
        fprintf(stdout, "[%s] Argument list too long\n", state->command->command);
        return 1;
    } else if (dc_error_is_errno(err, EACCES)) {
        fprintf(stdout, "[%s] Permission denied\n", state->command->command);
        return 2;
    } else if (dc_error_is_errno(err, EINVAL)) {
        fprintf(stdout, "[%s] Invalid argument\n", "state->command->command");
        return 3;
    } else if (dc_error_is_errno(err, ELOOP)) {
        fprintf(stdout, "[%s] Too many symbolic links encountered\n", state->command->command);
        return 4;
    } else if (dc_error_is_errno(err, ENAMETOOLONG)) {
        fprintf(stdout, "[%s] File name too long\n", state->command->command);
        return 5;
    } else if (dc_error_is_errno(err, ENOENT)) {
        fprintf(stdout, "[%s] No such file or directory\n", state->command->command);
        return 127;
    } else if (dc_error_is_errno(err, ENOTDIR)) {
        fprintf(stdout, "[%s] Not a directory\n", state->command->command);
        return 6;
    } else if (dc_error_is_errno(err, ENOEXEC)) {
        fprintf(stdout, "[%s] Exec format error\n", state->command->command);
        return 7;
    } else if (dc_error_is_errno(err, ENOMEM)) {
        fprintf(stdout, "[%s] Out of memory\n", state->command->command);
        return 8;
    } else if (dc_error_is_errno(err, ETXTBSY)) {
        fprintf(stdout, "[%s] Text file busy\n", state->command->command);
        return 9;
    } else {
        return 125;
    }
}

int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg) {
    struct state *state = arg;
    reset_state(env, err, state);
    return DC_FSM_EXIT;
}
