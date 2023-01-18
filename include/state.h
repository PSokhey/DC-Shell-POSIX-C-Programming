

#ifndef OPEN_STATE_H
#define OPEN_STATE_H

// States to be used for shell.
enum FSMState {
    INIT_STATE, // Initial shell state.
    READ_COMMANDS, // Reading command state.
    SEPARATE_COMMANDS, // Tokenize teh comm.
    PARSE_COMMANDS, // Parse the commands to shell for next action.
    EXECUTE_COMMANDS, // Execute the command that was parsed.
    RESET_STATE, // Clean memory and get ready for reading again.
    ERROR, // Error exit gracefully.
    EXIT, // Exit the program
    DESTROY_STATE // Clean memory before closing program.
};

#endif //OPEN_STATE_H
