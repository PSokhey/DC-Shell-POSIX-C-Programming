#include "command.h"
#include "shell.h"
#include "shell_impl.h"
#include "util.h"
#include <dc_posix/dc_unistd.h>

int parse_command(const struct dc_env *env, struct dc_error *err,
                  struct state *currentState) {

    // Check for any error till this point.
    if (hasErrorOccured(err, currentState, "Unable to parse command")) {
        return ERROR;
    }

    // for REGEX and word expandng.
    int REGEXResultErr;
    int REGXResultOut;
    int REGEXResultIn;
    regmatch_t REGEXMatch;
    int wordExpResult;
    wordexp_t wordExp;



    char *redirect;
    size_t redirect_len;

    if ((REGEXResultErr = regexec(currentState->err_redirect_regex, currentState->command->line, 1, &REGEXMatch, 0)) == REG_NOERROR) {
        redirect_len = REGEXMatch.rm_eo - REGEXMatch.rm_so;
        redirect = strndup(currentState->command->line + REGEXMatch.rm_so, redirect_len);
        if (redirect == NULL) {
            currentState->fatal_error = true;
            return ERROR;
        }
        if (strchr(redirect, '>') != NULL && strchr(redirect, '>')[1] == '>') {
            currentState->command->stderr_overwrite = true;
        }
        currentState->command->stderr_file = expand_path(env, err, redirect);
        if (currentState->command->stderr_file == NULL) {
            currentState->fatal_error = true;
            free(redirect);
            return ERROR;
        }
        free(redirect);
        currentState->command->line[REGEXMatch.rm_so] = '\0';
    }


    int err_redirect_result = regexec(currentState->err_redirect_regex,
                                      currentState->command->line, 1,
                                      &REGEXMatch, 0);
    if (err_redirect_result == 0) {
        size_t redirect_len = REGEXMatch.rm_eo - REGEXMatch.rm_so;
        char *redirect = strndup(currentState->command->line + REGEXMatch.rm_so, redirect_len);

        if (redirect == NULL) {
            currentState->fatal_error = true;
            return ERROR;
        }
        if (strstr(redirect, ">>")) {
            currentState->command->stderr_overwrite = true;
        }

        currentState->command->stderr_file = expand_path(env, err, redirect);

        if (currentState->command->stderr_file == NULL) {
            currentState->fatal_error = true;
            free(redirect);
            return ERROR;
        }
        free(redirect);
        currentState->command->line[REGEXMatch.rm_so] = '\0';
    }

    if (regexec(currentState->in_redirect_regex, currentState->command->line, 1, &REGEXMatch, 0) == 0) {
        size_t redirect_len = REGEXMatch.rm_eo - REGEXMatch.rm_so;
        char *redirect = strndup(currentState->command->line + REGEXMatch.rm_so, redirect_len);

        if (redirect == NULL) {
            currentState->fatal_error = true;
            return ERROR;
        }

        currentState->command->stdin_file = expand_path(env, err, redirect);

        if (currentState->command->stdin_file == NULL) {
            currentState->fatal_error = true;
            free(redirect);
            return ERROR;
        }

        free(redirect);
        currentState->command->line[REGEXMatch.rm_so] = '\0';
    }


    wordExpResult = dc_wordexp(env, err, currentState->command->line, &wordExp, 0);
    if (wordExpResult == 0) {

        currentState->command->argc = wordExp.we_wordc;
        currentState->command->argv = (char **) calloc(wordExp.we_wordc + 1, sizeof(char *));

        for (size_t i = 0; i < wordExp.we_wordc; ++i) {
            currentState->command->argv[i] = strdup(wordExp.we_wordv[i]);
        }

        currentState->command->argv[wordExp.we_wordc] = NULL;
        currentState->command->command = strdup(wordExp.we_wordv[0]);
        wordfree(&wordExp);

    } else {
        printf("unable to parse: %s\n", currentState->command->line);
    }
    return EXECUTE_COMMANDS;

}

void redirect(const struct dc_env *env, struct dc_error *err, struct state* currentState) {

    int fd;
    int open_flags;

    if (dc_error_has_error(err)){
        handle_error(env, err, currentState);
        return;
    }

    if (currentState->command->stdin_file != NULL){
        fd = open(currentState->command->stdin_file, O_RDONLY);
        if (fd == -1){
            perror("could not open file for redirect STDOUT");
            dc_error_has_error(err);
            close(fd);
            return;
        }
        dc_dup2(env, err, fd, STDIN_FILENO);
    }

    if (currentState->command->stdout_file != NULL){
        if (currentState->command->stderr_overwrite){
            open_flags = O_WRONLY | O_CREAT | O_TRUNC;
        } else{
            open_flags = O_WRONLY | O_CREAT | O_APPEND;
        }
        fd = open(currentState->command->stdout_file, open_flags, S_IRUSR | S_IWUSR);
        if (fd == -1){
            perror("could not open file for redirect STDIN");
            handle_error(env, err, currentState->command->command);
            close(fd);
            return;
        }
        dc_dup2(env, err, fd, STDOUT_FILENO);
    }

    if (currentState->command->stderr_file != NULL){
        if (currentState->command->stderr_overwrite){
            open_flags = O_WRONLY | O_CREAT | O_TRUNC;
        } else{
            open_flags = O_WRONLY | O_CREAT | O_APPEND;
        }
        fd = open(currentState->command->stderr_file, open_flags, S_IRUSR | S_IWUSR);
        if (fd == -1){
            handle_error(env, err, currentState->command->command);
            close(fd);
            return;
        }
        dc_dup2(env, err, fd, STDERR_FILENO);
    }

}




