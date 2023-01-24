#include "shell.h"

// Drive program the program.
int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[]) {

    // Run shell and get the return value.
    int returnValue;
    returnValue  = shell();

    return returnValue;
}
