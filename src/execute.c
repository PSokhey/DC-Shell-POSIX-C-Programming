#include "command.h"
#include "execute.h"
#include "shell.h"
#include "shell_impl.h"
#include "util.h"

// Create a child process to carry our command, then run command.
void execute(const struct dc_env *env, struct dc_error *err, struct state *currentState, char **path) {

    // For child process.
    pid_t childP = fork();
    int status;

    // If new process redirect.
    if (childP == 0) {
        redirect(env, err, currentState);
        if (dc_error_has_error(err)) {
            exit(126);
        }

        // call to run command on new process.
        run(env, err, currentState->command, path);
        status = handle_run_error(env, err, currentState->command->command);
        if (status != EXIT_SUCCESS){
            exit(status);
        }
    } else {

        // Wait for child process to finish.
        int exit_val;
        waitpid(childP, &exit_val, 0);
        currentState->command->exit_code = exit_val;
    }
}
