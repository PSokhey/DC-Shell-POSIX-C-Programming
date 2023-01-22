#include <command.h>



static struct command* createCommand(){

    // allocate new memory for the struct.
    struct command* newCommand = malloc(sizeof(struct command));

    // initial setup for the command struct.
    newCommand->command = NULL;
    newCommand->argc = 0;
    newCommand->argv = NULL;
    newCommand->exit_code = 0;
    newCommand->line = NULL;
    newCommand->std_overwrite = false;
    newCommand->stderr_overwrite = false;
    newCommand->stdin_file = NULL;
    newCommand->stdout_file = NULL;

    // Set the stdin_file field to redirect input from a file
    newCommand->stdin_file = open("input.txt", O_RDONLY);

    // Set the stdout_file field to redirect output to a file
    newCommand->stdout_file = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
}
