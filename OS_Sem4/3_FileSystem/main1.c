#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"

char* FindDirName(char* str, size_t* count) {
    size_t start = 0;
    size_t end = 0;
    size_t i = 0;
    while (str[i] != '\0') {
        if (str[i] == '/' && str[i+1] != '\0') {
            start = i+1;
            *count = 0;
        } else if (str[i] != '/') {
            (*count)++;
        }
        i++;
    }
    end = start + *count - 1;
    char* dirName = (char*)calloc(*count, sizeof(char));
    for (size_t i = start; i <= end; ++i) {
        dirName[end - i] = str[i];
    }
    return dirName;
}

int main(int argc, char** argv) {
    size_t dirSize = 0;
    struct stat* st  = {0};
    char* dirName = FindDirName(argv[1], &dirSize);
    if (stat(dirName, st) == 0) {

    } else {
        printf("ERROR: %s\n", strerror(errno));
    }
    // printf("%zu\n", dirSize);
    // for (size_t i = 0; i < dirSize; ++i) {
    //     printf("%c", dirName[i]);
    // }
    free(dirName);
    return 0;
}