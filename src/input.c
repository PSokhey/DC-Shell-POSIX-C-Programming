#include <dc_error/error.h>
#include <dc_env/env.h>
#include <bits/types/FILE.h>
#include <stdio.h>
#include "input.h"

/*
char *read_command_line(const struct dc_env *env, struct dc_error *err,
                        FILE *stream, size_t *line_size){
    char *line =  NULL;
    if (getline(&line, line_size, stream) == -1){
        if (feof(stream)){
            return NULL;
        }
        dc_error_has_error(err);
        return NULL;
    }
    return line;
}*/
