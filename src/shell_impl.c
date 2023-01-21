#include <shell_impl.h>
#include <string.h>
#include <util.h>

#define MAX_WORKING_DIR_LENGTH 4096


// example function of functions being used, to be moved later.
// currently are empty and just placeholders.
int init_state(const struct dc_env *env, struct dc_error *err, struct state* new_state){

/*    // Allocate memory for struct.
    struct state* new_state = malloc(sizeof(struct state));
    if (!new_state) {
        // handle allocation error
        return NULL;
    }*/

    // initialize all variables
    new_state->paths = NULL;
    new_state->prompt = NULL;
    new_state->current_line = NULL;
    new_state->command = NULL;
    new_state->fatal_error = false;
    new_state->max_line_length = 0;
    new_state->current_line_length = 0;

    // compile regular expressions
    int ret_code = regcomp(&new_state->in_redirect_regex, "[ \t\f\v]<.*", REG_EXTENDED);
    if (ret_code != 0) {
        // handle regular expression compilation error
        free(new_state);
        return NULL;
    }
    ret_code = regcomp(&new_state->out_redirect_regex, "[ \t\f\v][1^2]?>[>]?.*", REG_EXTENDED);
    if (ret_code != 0) {
        // handle regular expression compilation error
        regfree(&new_state->in_redirect_regex);
        free(new_state);
        return NULL;
    }
    ret_code = regcomp(&new_state->err_redirect_regex, "[ \t\f\v]2>[>]?.*", REG_EXTENDED);
    if (ret_code != 0) {
        // handle regular expression compilation error
        regfree(&new_state->in_redirect_regex);
        regfree(&new_state->out_redirect_regex);
        free(new_state);
        return NULL;
    }

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

    // set state.max_line_length to _SC_ARG_MAX via sysconf()
    new_state->max_line_length = sysconf(_SC_ARG_MAX);

    return READ_COMMANDS;
};
int read_commands(const struct dc_env *env, struct dc_error *err, struct state *currentState) {
    char commandInput[100] ;
    char workingDir[MAX_WORKING_DIR_LENGTH];


    if(getcwd(workingDir, sizeof (workingDir)) == NULL) {
        currentState->fatal_error = true;
        return ERROR;
    }

    // display working directory to the user with the assigned prompt.

    // Get the current working directory.
    printf("[%s] $ ",workingDir, currentState->prompt);
    scanf("%s", commandInput);
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
};
int destroy_state(const struct dc_env *env, struct dc_error *err, void *arg) {
    printf("Destroying state...\n");
    return DC_FSM_EXIT;
};

int handler_error(const struct dc_env *env, struct dc_error *err, void *arg);