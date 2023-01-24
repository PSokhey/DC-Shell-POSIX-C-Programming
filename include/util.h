#ifndef DC_SHELL_UTIL_H
#define DC_SHELL_UTIL_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <bits/types/FILE.h>
#include "state.h"

char *get_prompt(const struct dc_env *env, struct dc_error *err, void *arg);

char **get_path(const struct dc_env *env, struct dc_error *err, void *arg);

char *expand_path(const struct dc_env *env, struct dc_error *err, char *file);

//char **parse_path(const struct dc_env *env, struct dc_error *err, char *path_str);

void do_reset_state(const struct dc_env *env, struct dc_error *err, struct state *state);

//void display_state(const struct dc_env *env, const struct state *state, FILE *stream);

char *strCat(const char *str1, const char *str2);

#endif //DC_SHELL_UTIL_H
