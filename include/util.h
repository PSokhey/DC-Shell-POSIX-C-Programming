
#ifndef OPEN_UTIL_H
#define OPEN_UTIL_H

#include <regex.h>
#include <stdbool.h>
#include <command.h>

static char** split_string(const char* str, const char* delim);
static size_t count(const char *str, int c);

#endif //OPEN_UTIL_H
