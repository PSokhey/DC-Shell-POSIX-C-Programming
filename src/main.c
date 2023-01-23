#include <dc_env/env.h>
#include <dc_error/error.h>
#include <stddef.h>
#include <stdlib.h>
#include "shell.h"

int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]) {

    // Run shell and get the return value.
    int returnValue;
    returnValue  = shell();

    return returnValue;
}
