#ifndef DC_SHELL_INPUT_H
#define DC_SHELL_INPUT_H

#include <dc_error/error.h>
#include <dc_env/env.h>
#include <bits/types/FILE.h>

char *read_command_line(const struct dc_env *env, struct dc_error *err,
                        FILE *stream, size_t *line_size);

#endif //DC_SHELL_INPUT_H
