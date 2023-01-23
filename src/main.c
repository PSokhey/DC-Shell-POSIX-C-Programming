#include <dc_env/env.h>
#include <dc_error/error.h>
#include <stddef.h>
#include <stdlib.h>
#include "shell.h"

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]) {
    dc_env_tracer tracer;
    struct dc_env *env;
    struct dc_error *err;
    struct state *state;
    //int return_val = 0;

    // set the tracer to trace through the function calls
    //tracer = dc_env-default_tracer;
    tracer = NULL;

    err = dc_error_create(false);
    env = dc_env_create(err, false, tracer);

    //dc_error_init(err, false);
    dc_env_set_tracer(env, tracer);

    run_shell(env, err/*, state*/);

    return EXIT_SUCCESS;
}
