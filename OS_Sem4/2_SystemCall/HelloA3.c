#include <sys/syscall.h>
#include <unistd.h>

#include "stdio.h"

static ssize_t mywrite (int fd, const void *buf, size_t count)
{
    return syscall(SYS_write, fd, buf, count);
}

int main() {
    // printf("Hello world");
    mywrite(1, "Hello world!\n", 13);
    return 0;
}