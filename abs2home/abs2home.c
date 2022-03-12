#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PREFIX_LEN 6
#define TILT 1

bool _check_for_same_prefix(char *username, size_t username_len, const char * const original, size_t original_len)
{
    if (PREFIX_LEN + username_len > original_len) {
        return false;
    }

    char prefix[PREFIX_LEN + strlen(username)];
    strcpy(prefix, "/home/");
    strcat(prefix, username);
    // first character behind prefix must be '/' if string lengths differs
    if (PREFIX_LEN + username_len != original_len && original[PREFIX_LEN + username_len] != '/') {
        return false;
    }

    bool same_prefix = true;
    for (int i = 0; i < PREFIX_LEN + username_len; ++i) {
        if (prefix[i] != original[i]) {
            same_prefix = false;
            break;
        }
    }
    return same_prefix;
}

char *abs2home(char * const original)
{
    char username[64]; // Don't you dare have a longer username! :D
    getlogin_r(username, 63);
    size_t username_len = strlen(username);
    size_t original_len = strlen(original);
    if (!_check_for_same_prefix(username, username_len, original, original_len)) {
        return original;
    }

    char *result = (char *) malloc(original_len - (PREFIX_LEN + username_len - TILT) + 1);
    result[0] = '~';
    if (original_len - (PREFIX_LEN + username_len) == 1) {  // TODO: fix this hardcoding
        result[1] = '\0';
        return result;
    }

    int j = 1;  // result[0] == '~'
    for (int i = PREFIX_LEN + username_len; i < original_len; ++i) {
        result[j] = original[i];
        j++;
    }
    result[j] = '\0';
    return result;
}

int run(int argc, char *argv[])
{
    char *output = abs2home(argv[1]);
    printf("%s\n", output);
    return EXIT_SUCCESS;
}

#ifndef TESTING
int main(int argc, char *argv[])
{
    return run(argc, argv);
}
#endif
