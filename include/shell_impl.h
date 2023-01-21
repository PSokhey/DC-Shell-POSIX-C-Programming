// for the transfer of states to be here.

#ifndef OPEN_SHELL_IMPL_H
#define OPEN_SHELL_IMPL_H

#include <state.h> // loop issue if you can fix.
#include <stdio.h>

// example function of functions being used, to be moved later.
// currently are empty and just placeholders.
int init_state(const struct dc_env *env, struct dc_error *err, struct state *currentState);
int read_commands(const struct dc_env *env, struct dc_error *err, struct state *currentState);
int reset_state(const struct dc_env *env, struct dc_error *err, void *arg);
int separate_commands(const struct dc_env *env, struct dc_error *err, struct state *currentState);
int parse_commands(const struct dc_env *env, struct dc_error *err, void *arg);
int execute_commands(const struct dc_env *env, struct dc_error *err, void *arg);
int do_exit(const struct dc_env *env, struct dc_error *err, void *arg);
int handler_error(const struct dc_env *env, struct dc_error *err, void *arg);
int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg);

#endif //OPEN_SHELL_IMPL_H
