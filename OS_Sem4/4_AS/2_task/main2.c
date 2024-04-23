#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

void RecursiveStack() {
    int buf[4096];
    printf("pid: %d\n", getpid());
    usleep(100000);
    RecursiveStack();
}

void RecursiveHeap() {
    long size = 0;
    const int block = 1024*10;
    while(1) {
        int* a = malloc(block);
        size += block;
        printf("pid: %d; size %ld\n", getpid(), size);
        usleep(100000);
    }
}

int main() {
    printf("pid: %d\n", getpid());
    sleep(10);
    // RecursiveStack();
    RecursiveHeap();
    sleep(1000);
    return 0;
}