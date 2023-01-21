#include <shell_impl.h>


// example function of functions being used, to be moved later.
// currently are empty and just placeholders.
int init_state(const struct dc_env *env, struct dc_error *err, void *arg){
    printf("Initializing state...\n");
    return READ_COMMANDS;
};
int read_commands(const struct dc_env *env, struct dc_error *err, void *arg) {
    char command[100];
    printf("Enter a command: ");
    scanf("%s", command);
    return SEPARATE_COMMANDS;
};
int reset_state(const struct dc_env *env, struct dc_error *err, void *arg){
    printf("Resetting state..\n");
    return READ_COMMANDS;
};
int separate_commands(const struct dc_env *env, struct dc_error *err, void *arg){
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