#ifndef DC_SHELL_UTIL_H
#define DC_SHELL_UTIL_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <bits/types/FILE.h>
#include "state.h"

char **get_path();

char *get_prompt(const struct dc_env *env, struct dc_error *err, struct state* currentState);

char *expand_path(const struct dc_env *env, struct dc_error *err, char *file);

void do_reset_state(const struct dc_env *env, struct dc_error *err, struct state *currentStruct);

char *strCat(const char *str1, const char *str2);

#endif //DC_SHELL_UTIL_H
