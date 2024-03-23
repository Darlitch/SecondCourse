#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "string.h"
// #include "sys/types.h"
#include "sys/stat.h"
#include "dirent.h"

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

// void PrintError() {
//     size_t error_len = strerrorlen_s(errno) + 1;
//     char error_buf[error_len];
//     strerror_s(error_buf, error_len, errno);
//     fprintf(stderr, "Error message : %s\n", error_buf);
// }
int IsDir(char* dirName, struct dirent* entry, size_t lenDir) {
    struct stat st = {0};
    char* fileName;
    const size_t lenFile = strlen(entry->d_name);
    if (dirName[lenDir-1] == '/') {
        fileName = malloc(lenDir + lenFile + 1);
        memcpy(fileName, dirName, lenDir);
    } else {
        fileName = malloc(lenDir + lenFile + 2);
        memcpy(fileName, dirName, lenDir);
        fileName[lenDir] = '/';
        lenDir++;
    }
    memcpy(fileName + lenDir, entry->d_name, lenFile + 1);
    stat(fileName, &st);
    if (S_ISREG(st.st_mode) == 1) {
        return 1;
    }
    return 0;
}

void CopyFiles(char* dirName, char* dirNameReverse) {
    DIR* dir;
    struct dirent* entry;
    struct stat st  = {0};
    dir = opendir(dirName);
    const size_t lenDir = strlen(dirName);
    if (dir != NULL) {
        while((entry = readdir(dir)) != NULL) {
            if (IsDir(dirName, entry, lenDir)) {
                printf ("%s\n", entry->d_name);
            }
        }
    } else {
        printf("ERROR: Couldn't open a directory!\n");
    }
}

int main(int argc, char** argv) {
    size_t dirSize = 0;
    struct stat st  = {0};
    errno = 0;
    char* dirName = FindDirName(argv[1], &dirSize);
    if (stat(argv[1], &st) == 0 && S_ISREG(st.st_mode) == 0) {
        // if (mkdir(dirName, 0755) == -1) {
        //     printf("ERROR: Couldn't create a directory!\n");
        // }
        CopyFiles(argv[1], dirName);
        printf("ok");
    } else {
        // printf("ERROR: %s\n", strerror(errno));
        // PrintError();
        printf("ERROR: The directory does not exist!\n");
    }
    // printf("%zu\n", dirSize);
    // for (size_t i = 0; i < dirSize; ++i) {
    //     printf("%c", dirName[i]);
    // }
    free(dirName);
    return 0;
}