#include <shell_impl.h>
#include <dc_posix/dc_stdio.h>
#include <dc_util/strings.h>
#include <dc_util/filesystem.h>
#include <string.h>
#include <util.h>

#define MAX_WORKING_DIR_LENGTH 4096


// example function of functions being used, to be moved later.
// currently are empty and just placeholders.
//CLEAR
int init_state(const struct dc_env *env, struct dc_error *err, struct state* new_state){

    // initial error handling
    if (checkError(err, new_state,"Error before initializing state")) {
        return ERROR;
    }

    // set state.max_line_length to _SC_ARG_MAX via sysconf()
    regcomp(&new_state->in_redirect_regex , "[ \t\f\v]<.*", REG_EXTENDED);
    regcomp(&new_state->out_redirect_regex , "[ \t\f\v][1^2]?>[>]?.*", REG_EXTENDED);
    regcomp(&new_state->err_redirect_regex , "[ \t\f\v]2>[>]?.*", REG_EXTENDED);


    // get the PATH environment variable
    char* path_env = getenv("PATH");
    if (path_env != NULL) {
        // split PATH into an array, separate by :
        printf("%s\n",path_env);
        new_state->paths = split_string(path_env,  ':'); // NEED TO IMPLIMINT


        //for testing path variable
        //printf("%s",new_state->paths[0]);
        //printf("%s",new_state->paths[1]);

    }

    // get the PS1 environment variable
    char* ps1_env = getenv("PS1");
    if (ps1_env != NULL) {
        // set state.prompt to the PS1 value
        new_state->prompt = strdup(ps1_env);
    } else {
        // set state.prompt to " $ "
        new_state->prompt = strdup(" $ ");
    }

    return READ_COMMANDS;
};
int read_commands(const struct dc_env *env, struct dc_error *err, struct state *currentState) {
    size_t cmdLineLength = 0;
    char *workingDir;

    workingDir = dc_get_working_dir(env, err);
    if(checkError(err, currentState, "Could not get working directory")) {
        return ERROR;
    }

    // Get the current working directory and display to user.
    fprintf(stdout,"[%s] $ ",workingDir, currentState->prompt);
    //scanf("%s", commandInput);

    // allocating memory for taking input.
    currentState->current_line = malloc(sizeof (char));
    if(checkError(err, currentState,"Could not allocate memory.")) {
        return ERROR;
    }

    // taking the input.
    dc_getline(env, err, &currentState->current_line, &cmdLineLength, stdin);
    if(checkError(err, currentState, "Could not get command input.")) {
        return ERROR;
    }

    dc_str_trim(env, currentState->current_line);
    printf("Command: %s\n", currentState->current_line);
    cmdLineLength = strlen(currentState->current_line);
    if(cmdLineLength == 0) {
        return RESET_STATE;
    }

    // When the user actually ends a command.
    currentState->current_line_length = cmdLineLength;
    return SEPARATE_COMMANDS;
};
int reset_state(const struct dc_env *env, struct dc_error *err, void *arg){
    printf("Resetting state..\n");
    return READ_COMMANDS;
};
int separate_commands(const struct dc_env *env, struct dc_error *err, struct state *currentState){
    printf("Seperating commands...\n");
    return PARSE_COMMANDS;
};
int parse_commands(const struct dc_env *env, struct dc_error *err, void *arg){
    printf("Parsing commands...\n");
    return EXECUTE_COMMANDS;
};
int execute_commands(const struct dc_env *env, struct dc_error *err, void *arg){
    printf("Executing commands...\n");
    return RESET_STATE;
};
int do_exit(const struct dc_env *env, struct dc_error *err, void *arg){
    printf("Exiting program...");
    return DESTROY_STATE;
};
int handler_error(const struct dc_env *env, struct dc_error *err, void *arg){
    printf("Handling error...\n");

    // temporary to keep fsm going and needs to be properly handled.
    return READ_COMMANDS;
};
int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg) {
    printf("Destroying state...\n");
    return DC_FSM_EXIT;
}

// check if there is an error or not.
bool checkError(struct dc_error* err, struct state* currentState,const char* errorMessage) {
    if (err != NULL && dc_error_has_error(err)) {
        currentState->fatal_error = true;
        perror(errorMessage);
        return true;
    } else
        return false;
}