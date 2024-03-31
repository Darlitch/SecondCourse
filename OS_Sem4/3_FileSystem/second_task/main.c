#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "dirent.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
 	
#include "locale.h"

void PrintMenu() {
    printf("1. создать каталог, указанный в аргументе\n");
    printf("2. вывести содержимое каталога, указанного в аргументе\n");
    printf("3. удалить каталог, указанный в аргументе\n");
    printf("4. создать файл, указанный в аргументе\n");
    printf("5. вывести содержимое файла, указанного в аргументе\n");
    printf("6. удалить файл, указанный в аргументе\n");
    printf("7. создать символьную ссылку на файл, указанный в аргументе\n");
    printf("8. вывести содержимое символьной ссылки, указанный в аргументе\n");
    printf("9. вывести содержимое файла, на который указывает символьная ссылка, указанная в аргументе\n");
    printf("10. удалить символьную ссылку на файл, указанный в аргументе\n");
    printf("11. создать жесткую ссылку на файл, указанный в аргументе\n");
    printf("12. удалить жесткую ссылку на файл, указанный в аргументе\n");
    printf("13. вывести права доступа к файлу, указанному в аргументе и количество жестких ссылок на него\n");
    printf("14. изменить права доступа к файлу, указанному в аргументе\n");
}

void ClearBuffer() {
    char ch;
    while (ch != '\n') {
        ch = getchar();
    }
}

void CreateDir(const char* dirName) {
    if (mkdir(dirName, 0755) == -1) {
        fprintf(stderr, "ERROR: Couldn't create a directory!\n");
        exit(-1);
    }
}

char* TakeFileName(const char* dirName, const char* fileN, size_t lenDir) {
    char* fileName;
    const size_t lenFile = strlen(fileN);
    if (dirName[lenDir-1] == '/') {
        fileName = malloc(lenDir + lenFile + 1);
        memcpy(fileName, dirName, lenDir);
    } else {
        fileName = malloc(lenDir + lenFile + 2);
        memcpy(fileName, dirName, lenDir);
        fileName[lenDir] = '/';
        lenDir++;
    }
    memcpy(fileName + lenDir, fileN, lenFile + 1);
    return fileName;
}

void ViewCatalog(const char* dirName) {
    DIR* dir;
    struct dirent* entry;
    struct stat st  = {0};
    dir = opendir(dirName);
    const size_t lenDir = strlen(dirName);
    if (dir != NULL) {
        while((entry = readdir(dir)) != NULL) {
            char* fileName = TakeFileName(dirName, entry->d_name, lenDir);
            stat(fileName, &st);
            if (S_ISREG(st.st_mode) == 1) {
                printf("reg: %s\n", entry->d_name);
            } else {
                printf("dir: %s\n", entry->d_name);
            }
        }
    } else {
        fprintf(stderr, "ERROR: Couldn't open a directory!\n");
        exit(-2);
    }
    closedir(dir);
}

void DeleteFile(const char* fileName) {
    if (unlink(fileName) != 0) {
        fprintf(stderr, "ERROR DF: the file could not be deleted");
        exit(-6);
    }
}

void DeleteDir(const char* dirName) {
    DIR* dir;
    struct stat st = {0};
    struct dirent* entry;
    dir = opendir(dirName);
    printf("1\n");
    const size_t lenDir = strlen(dirName);
    if (dir != NULL) {
        while((entry = readdir(dir)) != NULL) {
            char* fileName = TakeFileName(dirName, entry->d_name, lenDir);
            stat(fileName, &st);
            if (S_ISREG(st.st_mode) == 1) {
                DeleteFile(fileName);
            } else if (!strcmp(entry->d_name, ".") && !strcmp(entry->d_name, "..")) {
                DeleteDir(fileName);
            }
        }
        if (!rmdir(dirName)) {
            fprintf(stderr, "ERROR DD: the folder could not be deleted");
        }
    } else {
        fprintf(stderr, "ERROR: Couldn't open a directory!\n");
        exit(-3);
    }
    closedir(dir);
}

void CreateFile(const char* fileName) {
    int file = creat(fileName, 0700);
    if (file < 0) {
        fprintf(stderr, "ERROR: Can`t create output file");
        exit(-4);
    }
    close(file);
}

void Cat(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    int ch;
    if (file == NULL) {
        fprintf(stderr, "ERROR Cat: can`t open file");
        exit(-5);
    }
    while(!feof(file) && !ferror(file)) {
        ch = getc(file);
        if (ch != EOF) {
            putchar(ch);
        }
    }
    putchar('\n');
}

void Menu(const char* fileName) {
    size_t menu = 100;
    while(menu != 0) {
        PrintMenu();
        scanf("%zu", &menu);
        putchar('\n');
        ClearBuffer();
        switch (menu) {
        case 1:
            CreateDir(fileName);
            break;
        case 2:
            ViewCatalog(fileName);
            break;
        case 3:
            DeleteDir(fileName);
            break;
        case 4:
            CreateFile(fileName);
        case 5:
            Cat(fileName);
            break;
        case 6:
            DeleteFile(fileName);
        default:
            break;
        }
        printf("\n\n");
    }
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "Rus");
    if (argc != 2) {
        fprintf(stderr, "ERROR: Incorrect number of arguments");
        exit(-15);
    }
    Menu(argv[1]);
    return 0;
}