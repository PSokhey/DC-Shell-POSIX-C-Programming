#include <string.h>
#include "state.h"
#include <dc_env/env.h>
#include <dc_error/error.h>
#include <malloc.h>
#include <stdlib.h>
#include "shell.h"
#include "util.h"

char **get_path(const struct dc_env *env, struct dc_error *err, void *arg)
{
    char *path_env = getenv("PATH");
    const char * delimiter = ":";
    char * tokenized_path = strtok(path_env, delimiter);
    char **path = NULL;
    unsigned rows = 0;
    while (tokenized_path){
        path = realloc(path, (rows + 1) * sizeof(path));
        path[rows] = malloc(strlen(tokenized_path) + 1);
        strcpy(path[rows], tokenized_path);
        rows++;
        tokenized_path = strtok(NULL, delimiter);
    }
    path = realloc(path, (rows + 1) * sizeof(path));
    path[rows] = NULL;
    return path;
}

char *get_prompt(const struct dc_env *env, struct dc_error *err, void *arg){
    struct state * state = (struct state * )arg;
    char *ps1_env = getenv("PS1");
    if (ps1_env == NULL) {
        state->prompt = (char *) malloc(sizeof(char) * 3);
        strcpy(state->prompt, "$ ");
    } else {
        size_t ps1_len = strlen(ps1_env);
        state->prompt = (char *) malloc(sizeof(char) * (ps1_len + 1));
        strcpy(state->prompt, ps1_env);
    }
    return ps1_env;
}

char **parse_path(const struct dc_env *env, struct dc_error *err, char *path_str){
    char **path = NULL;
    int path_len = 0;
    char *token = strtok(path_str, "/");
    while (token != NULL){
        path = realloc(path, sizeof (char *) * path_len);
        if (path == NULL){
            dc_error_has_error(err);
            return NULL;
        }
        path[path_len - 1] = token;
        token = strtok(NULL, "/");
    }

    path = realloc(path, sizeof (char *) * (path_len + 1));
    path[path_len] = 0;

    return path;
}

void do_reset_state(const struct dc_env *env,
                    struct dc_error *err, struct state *state) {

    free(state->current_line);
    state->current_line = NULL;
    memset(err, 0, sizeof(state));
}

char *expand_path(const struct dc_env *env, struct dc_error *err, char *file){
    if (file[0] == '~'){
        char *home_directory = getenv("HOME");
        if (home_directory == NULL){
            printf("ERROR: HOME environment variable not found\n");
            return NULL;
        }
        size_t home_directory_len = strlen(home_directory);
        size_t file_len = strlen(file);
        char *expanded_file = malloc(home_directory_len + file_len);
        if (expanded_file == NULL){
            printf("ERROR: failed to allocate memory for expanded file\n");
            return NULL;
        }
        strcpy(expanded_file, home_directory);
        strcpy(expanded_file + home_directory_len, file + 1);
        return expanded_file;
    } else if (file[0] == '.'){
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL){
            size_t cwd_len = strlen(cwd);
            size_t file_len = strlen(file);
            char *expanded_file = malloc(cwd_len + file_len);
            if (expanded_file == NULL){
                printf("ERROR: failed to allocate memory for expanded file\n");
                return NULL;
            }
            strcpy(expanded_file, cwd);
            strcpy(expanded_file + cwd_len, file + 1);
            return expanded_file;
        }
    } else {
        return file;
    }
}

char *my_strcat(const char *str1, const char *str2){
    char *dest = NULL;
    size_t str1_length, str2_length;

    if(str1 && str2)
    {
        dest = malloc((str1_length = strlen(str1)) + (str2_length = strlen(str2)) + 1);
        if(dest)
        {
            memcpy(dest, str1, str1_length);
            memcpy(dest + str1_length, str2, str2_length);
        }
        dest[(str1_length + str2_length)] = '\0';
    }
    return dest;
}







