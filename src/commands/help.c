/* help.c - comando help */
#include "commands.h"

extern void println(const char* str);
extern void print(const char* str);

int cmd_help(int argc, char *argv[]) {
    (void)argc; (void)argv;

    println("Comandos disponiveis:");
    for (int i = 0; i < num_commands; i++) {
        print(commands[i].name);
        print(" - ");
        println(commands[i].desc);
    }
    return 0;
}