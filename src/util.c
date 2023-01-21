
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

char** split_string(const char* str, const char* delim) {

    char* strTest = strdup("ah:ke:ef"); // only for testing and should be deleted later.
    char*token;
    size_t numOfTokens;
    char** result;  // resulting list of paths

    // count the number of tokens
    numOfTokens = count(strTest, delim);
    printf("%d",numOfTokens);

    return result;
}

