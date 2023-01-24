#include <dc_env/env.h>
#include <dc_error/error.h>
#include <dc_posix/dc_unistd.h>
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "builtins.h"

// Changing directory with build in command.
void builtin_cd(const struct dc_env *env, struct dc_error *err, struct state *currentState) {

    char *path;
    if (currentState->command->argv[1] == NULL) {
        dc_expand_path(env, err, &path, "~/");
        dc_chdir(env, err, path);
    } else {
        if (strcmp(currentState->command->argv[1], "/") == 0) {
            path = strdup("/");
        } else if (strcmp(currentState->command->argv[1], "..") == 0) {
            path = strdup("../");
        } else if (strcmp(currentState->command->argv[1], ".") == 0) {
            path = strdup("./");
        } else if (strcmp(currentState->command->argv[1], "~") == 0) {
            path = strdup("~/");
        } else {
            path = strdup(currentState->command->argv[1]);
        }
        dc_chdir(env, err, currentState->command->argv[1]);
        path = strdup(currentState->command->argv[1]);
    }

    if (dc_error_has_error(err)) {
        if (dc_error_is_errno(err, EACCES)) {
            fprintf(stdout, "%s: Permission denied\n", path);

        } else if (dc_error_is_errno(err, ELOOP)) {
            fprintf(stdout, "%s: Too many symbolic links encountered\n", path);

        } else if (dc_error_is_errno(err, ENAMETOOLONG)) {
            fprintf(stdout, "%s: File name too long\n", path);

        } else if (dc_error_is_errno(err, ENONET)) {
            fprintf(stdout, "%s: No such file or directory\n", path);

        } else if (dc_error_is_errno(err, ENOTDIR)) {
            fprintf(stdout, "%s: Not a directory\n", path);
        }
        currentState->command->exit_code = 1;
    } else
        currentState->command->exit_code = 0;

    free(path);
}