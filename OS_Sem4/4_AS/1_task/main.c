#include "stdio.h"
#include "unistd.h"

int c = 5;
char d = 'a';

int e;
char g;

const int m;
const char n;

void LocalVar() {
    int la;
    char lb;
    printf("LocalVar adress: la - %p, lb - %p\n", &la, &lb);
}

void StaticVar() {
    static int sa;
    static char sb;
    printf("StaticVar adress: sa - %p, sb - %p\n", &sa, &sb);
}

void ConstVar() {
    const int ca;
    const char cb;
    printf("ConstVar adress: ca - %p, cb - %p\n", &ca, &cb);
}

void GlobalVar() {
    printf("GlobalInitVar adress: c - %p, d - %p\n", &c, &d);
    printf("GlobalNonInitVar adress: e - %p, g - %p\n", &e, &g);
    printf("ConstGlobalVar adress: m - %p, n - %p\n", &m, &n);
}

void InitLocalVar() {
    int ila;
    ila = 5;
    return
}

int main() {
    printf("pid: %d\n", getpid());
    LocalVar();
    StaticVar();
    ConstVar();
    GlobalVar();
    getchar();
}