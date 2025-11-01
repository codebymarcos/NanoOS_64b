/* cd.c - comando cd */
#include "commands.h"

extern void println(const char* str);
extern void print(const char* str);

/* Diretório atual simulado (global simples) */
static char current_dir[256] = "/";

int cmd_cd(int argc, char *argv[]) {
    if (argc < 2) {
        println("Uso: cd <diretorio>");
        return -1;
    }

    /* Simulação simples: apenas muda se for "/" ou ".." */
    if (strcmp(argv[1], "/") == 0) {
        strcpy(current_dir, "/");
    } else if (strcmp(argv[1], "..") == 0) {
        if (strcmp(current_dir, "/") != 0) {
            char *last = strrchr(current_dir, '/');
            if (last && last != current_dir) {
                *last = '\0';
            } else {
                strcpy(current_dir, "/");
            }
        }
    } else {
        /* Simular mudança para subdir */
        if (strcmp(current_dir, "/") == 0) {
            strcpy(current_dir, "/");
            strcat(current_dir, argv[1]);
        } else {
            strcat(current_dir, "/");
            strcat(current_dir, argv[1]);
        }
    }

    print("Diretorio atual: ");
    println(current_dir);
    return 0;
}