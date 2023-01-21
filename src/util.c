
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function for split_string to count the number of tokens.
static size_t count(const char *str, int c) {
    size_t num = 0;

    for(const char *temp = str; *temp; temp++) {
        if(*temp == c) {
            num++;
        }
    }

    return num;
}

char** split_string(const char* path, const char* delim) {
    //char* str = strdup("aa:dff:dfe");
    char *state;
    char* token;
    size_t num;
    char **list;
    size_t i;

    state = path;
    num = count(path, delim);
    //printf("Number: %d\n",num);
    list = calloc( num + 2, sizeof(char*));
    i = 0;

    while((token = strtok_r(state, ":", &state)) != NULL) {
        list[i] = strdup(token);
        i++;
    }

    // for test purposes only.
/*    printf("%s\n",list[0]);
    printf("%s\n",list[1]);
    printf("%s\n",list[2]);*/


    list[i] = NULL;
    //free(str);



    return list;
}





