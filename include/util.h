#ifndef DC_SHELL_UTIL_H
#define DC_SHELL_UTIL_H

#include <dc_env/env.h>
#include <dc_error/error.h>
#include <bits/types/FILE.h>
#include "state.h"

/**
 * Get the path variable.
 * @return array for the path.
 */
char **get_path();

/**
 * get the prompt for the command line.
 * @param env pointer to environment object.
 * @param err pointer to the error object.
 * @param currentState pointer to the state object.
 * @return the prompt string.
 */
char *get_prompt(const struct dc_env *env, struct dc_error *err, struct state* currentState);

/**
 * expanded path for executing commands.
 * @param env pointer to the environment object.
 * @param err pointer to the error object.
 * @param file pointer to the file.
 * @return
 */
char *expand_path(const struct dc_env *env, struct dc_error *err, char *file);

/**
 * TO reset the state object for a new command.
 * @param env pointer to the environment object.
 * @param err pointer to the error object.
 * @param currentStruct pointer to the state object.
 */
void do_reset_state(const struct dc_env *env, struct dc_error *err, struct state *currentStruct);

/**
 * custom string catinating.
 * @param str1 string one.
 * @param str2 string two.
 * @return catinated string.
 */
char *strCat(const char *str1, const char *str2);

#endif //DC_SHELL_UTIL_H
