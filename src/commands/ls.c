/* ls.c - comando ls */
#include "commands.h"

extern void println(const char* str);

/* Lista simulada de arquivos */
static const char *files[] = {
    "kernel.bin",
    "kernel.iso",
    "src/",
    "Makefile",
    "progress.txt"
};

int cmd_ls(int argc, char *argv[]) {
    (void)argc; (void)argv;  /* ignorar args */

    println("Arquivos no diretorio:");
    for (int i = 0; i < 5; i++) {
        println(files[i]);
    }
    return 0;
}