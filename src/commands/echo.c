/* echo.c - comando echo */
#include "commands.h"

extern void println(const char* str);
extern void print(const char* str);

int cmd_echo(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        print(argv[i]);
        if (i < argc - 1) print(" ");
    }
    println("");
    return 0;
}