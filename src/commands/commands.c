/* commands.c - implementação dos comandos */
#include "commands.h"

/* Implementações simples de string.h para ambiente freestanding */
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

char* strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

char* strcat(char *dest, const char *src) {
    char *d = dest;
    while (*d) d++;
    while ((*d++ = *src++));
    return dest;
}

char* strrchr(const char *s, int c) {
    const char *last = 0;
    while (*s) {
        if (*s == c) last = s;
        s++;
    }
    return (char*)last;
}

char* strtok(char *str, const char *delim) {
    static char *saved = 0;
    if (str) saved = str;
    if (!saved) return 0;
    
    /* pular delimitadores */
    while (*saved && strchr(delim, *saved)) saved++;
    if (!*saved) return 0;
    
    char *token = saved;
    /* encontrar fim do token */
    while (*saved && !strchr(delim, *saved)) saved++;
    if (*saved) *saved++ = '\0';
    
    return token;
}

const char* strchr(const char *s, int c) {
    while (*s) {
        if (*s == c) return s;
        s++;
    }
    return 0;
}

/* Declarações dos comandos */
int cmd_cd(int argc, char *argv[]);
int cmd_ls(int argc, char *argv[]);
int cmd_echo(int argc, char *argv[]);
int cmd_help(int argc, char *argv[]);
int cmd_exit(int argc, char *argv[]);

/* Lista de comandos */
command_t commands[] = {
    {"cd", cmd_cd, "Muda o diretório atual"},
    {"ls", cmd_ls, "Lista arquivos no diretório"},
    {"echo", cmd_echo, "Imprime argumentos"},
    {"help", cmd_help, "Mostra ajuda"},
    {"exit", cmd_exit, "Sai do kernel"}
};

const int num_commands = sizeof(commands) / sizeof(commands[0]);

/* Função para executar comando */
int execute_command(const char *cmd_line) {
    char line[256];
    strcpy(line, cmd_line);

    /* Parse simples: dividir em tokens */
    char *argv[10];
    int argc = 0;
    char *token = strtok(line, " \t\n");
    while (token && argc < 10) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    if (argc == 0) return 0;

    /* Encontrar comando */
    for (int i = 0; i < num_commands; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            return commands[i].func(argc, argv);
        }
    }

    /* Comando não encontrado */
    extern void println(const char* str);
    println("Comando nao encontrado. Digite 'help' para lista.");
    return -1;
}