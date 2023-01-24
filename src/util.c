#include <string.h>
#include "state.h"
#include <dc_env/env.h>
#include <dc_error/error.h>
#include <malloc.h>
#include <stdlib.h>
#include "shell.h"
#include "util.h"

// Get the array for the path variable.
char **getPath()
{
    char *pathEnv = getenv("PATH");
    const char * delimiter = ":";
    char * tokenized_path = strtok(pathEnv, delimiter);
    char **pathList = NULL;
    unsigned rows = 0;

    // Tokenize the pathList to put into an array.
    while (tokenized_path){
        pathList = realloc(pathList, (rows + 1) * sizeof(pathList));
        pathList[rows] = malloc(strlen(tokenized_path) + 1);
        strcpy(pathList[rows], tokenized_path);
        rows++;
        tokenized_path = strtok(NULL, delimiter);
    }

    // Last entry is NULL.
    pathList = realloc(pathList, (rows + 1) * sizeof(pathList));
    pathList[rows] = NULL;

    // Return list for path.
    return pathList;
}

// Get the prompt for the command line input.
char *getPrompt(const struct dc_env *env, struct dc_error* err, struct state* currentState){
    char *ps1Env = getenv("PS1");
    if (ps1Env == NULL) {
        currentState->prompt = strdup("$ ");
    } else {
        currentState->prompt = strdup(ps1Env);
    }
    return currentState->prompt;
}

char *expand_path(const struct dc_env *env, struct dc_error *err, char *file) {
    char *expanded_file = NULL;
    if (file[0] == '~') {
        char *home_directory = getenv("HOME");
        if (home_directory == NULL) {
            fprintf(stderr, "Unable to find HOME environment.\n");
            return NULL;
        }
        expanded_file = malloc(strlen(home_directory) + strlen(file) - 1);
        if (expanded_file == NULL) {
            fprintf(stderr, "Failed to allocate memory for expanded path.\n");
            return NULL;
        }
        strcpy(expanded_file, home_directory);
        strcat(expanded_file, file + 1);
    } else if (file[0] == '.') {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            expanded_file = malloc(strlen(cwd) + strlen(file));
            if (expanded_file == NULL) {
                fprintf(stderr, "Failed to allocate memory for expanded path.\n");
                return NULL;
            }
            strcpy(expanded_file, cwd);
            strcat(expanded_file, file + 1);
        }
    } else {
        expanded_file = file;
    }
    return expanded_file;
}

char *strCat(const char *str1, const char *str2) {
    char *target;
    size_t str1_length, str2_length;

    // Check that str1 and str2 are not null
    if (!str1 || !str2) {
        return NULL;
    }

    // Get the lengths of str1 and str2
    str1_length = strlen(str1);
    str2_length = strlen(str2);

    // Allocate memory for the concatenated string
    target = malloc(str1_length + str2_length + 1);

    // Check that memory allocation was successful
    if (!target) {
        return NULL;
    }

    // Copy the contents of str1 and str2 into target
    memcpy(target, str1, str1_length);
    memcpy(target + str1_length, str2, str2_length);

    // Null-terminate the concatenated string
    target[str1_length + str2_length] = '\0';

    return target;
}

// Reseting the command struct.
void do_reset_state(const struct dc_env *env,
                    struct dc_error *err, struct state *currentState) {

    // reset the state properties.
    free(currentState->current_line);
    free(currentState->command->command);
    free(currentState->command->stdin_file);
    free(currentState->command->stdout_file);
    free(currentState->command->stderr_file);
    for (size_t i = 0; i < currentState->command->argc; ++i) {
        free(currentState->command->argv[i]);
    }
    free(currentState->command->argv);
    free(currentState->command);


    currentState->current_line = NULL;
    memset(err, 0, sizeof(currentState));
}












