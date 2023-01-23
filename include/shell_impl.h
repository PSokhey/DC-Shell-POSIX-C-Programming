// for the transfer of states to be here.

#ifndef OPEN_SHELL_IMPL_H
#define OPEN_SHELL_IMPL_H

#include <stdbool.h>
#include <dc_error/error.h>
#include <dc_posix/dc_stdio.h>
#include <dc_util/strings.h>
#include <dc_util/filesystem.h>
#include <util.h>
#include <state.h> // loop issue if you can fix.
#include <stdio.h>
#include <command.h>
#include <string.h>


// example function of functions being used, to be moved later.
// currently are empty and just placeholders.
int init_state(const struct dc_env *env, struct dc_error *err, struct state *currentState);
int read_commands(const struct dc_env *env, struct dc_error *err, struct state *currentState);
int reset_state(const struct dc_env *env, struct dc_error *err, void *arg);
int separate_commands(const struct dc_env *env, struct dc_error *err, struct state *currentState);
int parse_command(const struct dc_env *env, struct dc_error *err, struct state *currentState);
int parse_commands(const struct dc_env *env, struct dc_error *err, struct state *currentState);
int execute_commands(const struct dc_env *env, struct dc_error *err, void *arg);
int do_exit(const struct dc_env *env, struct dc_error *err, void *arg);
int handler_error(const struct dc_env *env, struct dc_error *err, void *arg);
int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg);

bool checkError(struct dc_error* err, struct state* currentState,const char* errorMessage);

static struct command* createCommand();

void destoryCommandStruct(struct command);

#endif //OPEN_SHELL_IMPL_H
