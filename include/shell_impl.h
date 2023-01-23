#ifndef DC_SHELL_SHELL_IMPL_H
#define DC_SHELL_SHELL_IMPL_H

#include <dc_env/env.h>
#include <dc_error/error.h>

int init_state(const struct dc_env *env, struct dc_error *err, struct state* currentState);

int destroy_state(const struct dc_env *env, struct dc_error *err, struct state* currentState);

int reset_state(const struct dc_env *env, struct dc_error *error, struct state* currentState);

int read_commands(const struct dc_env *env, struct dc_error *err, struct state* currentState);

int separate_commands(const struct dc_env *env, struct dc_error *err, struct state* currentState);

int parse_commands(const struct dc_env *env, struct dc_error *err, struct state* currentState);

int execute_commands(const struct dc_env *env, struct dc_error *err, struct state* currentState);

int do_exit(const struct dc_env *env, struct dc_error *err, struct state* currentState);

int handle_error(const struct dc_env *env, struct dc_error *err, struct state* currentState);

int handle_run_error(const struct dc_env *env, struct dc_error *err, struct state* currentState);

bool hasErrorOccured(struct dc_error* err, struct state* currentState, char* errorMessage);

#endif //DC_SHELL_SHELL_IMPL_H
