/* exit.c - comando exit */
#include "commands.h"

extern void println(const char* str);

int cmd_exit(int argc, char *argv[]) {
    (void)argc; (void)argv;

    println("Saindo do kernel...");
    /* Halt */
    while (1) __asm__ volatile("hlt");
    return 0;  /* nunca chega aqui */
}