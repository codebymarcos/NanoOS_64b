/* commands.h - header para comandos do shell kernel */
#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define NULL ((void*)0)

/* Funções de string para freestanding */
int strcmp(const char *s1, const char *s2);
char* strcpy(char *dest, const char *src);
char* strcat(char *dest, const char *src);
char* strrchr(const char *s, int c);
char* strtok(char *str, const char *delim);
const char* strchr(const char *s, int c);

/* Estrutura para um comando */
typedef struct {
    const char *name;
    int (*func)(int argc, char *argv[]);
    const char *desc;
} command_t;

/* Lista de comandos disponíveis */
extern command_t commands[];

/* Número de comandos */
extern const int num_commands;

/* Função para executar comando por nome */
int execute_command(const char *cmd_line);

#endif /* COMMANDS_H */