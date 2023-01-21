

#ifndef OPEN_SHELL_H
#define OPEN_SHELL_H

#include <stdio.h>
#include <state.h>
#include <shell_impl.h>
#include <dc_fsm/fsm.h>
#include <dc_posix/dc_time.h>
#include <ctype.h>
#include <string.h>
#include <dc_error/error.h>


// runs the changing of the shell state.
void run_shell();

#endif //OPEN_SHELL_H
