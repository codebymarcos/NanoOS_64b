/* help.c - comando help */
#include "commands.h"
#include "colors.h"

extern void println(const char* str);
extern void print(const char* str);

/* Imprime com cor */
static void print_color(const char* color, const char* text) {
    print(color);
    print(text);
    print(ANSI_RESET);
}

int cmd_help(int argc, char *argv[]) {
    (void)argc; (void)argv;

    print_color(ANSI_CYAN ANSI_BOLD, "╔══════════════════════════════════════════╗\n");
    print_color(ANSI_CYAN ANSI_BOLD, "║");
    print_color(ANSI_YELLOW ANSI_BOLD, "           COMANDOS DISPONÍVEIS");
    print_color(ANSI_CYAN ANSI_BOLD, "           ║\n");
    print_color(ANSI_CYAN ANSI_BOLD, "╚══════════════════════════════════════════╝\n");
    println("");
    
    for (int i = 0; i < num_commands; i++) {
        print_color(ANSI_GREEN ANSI_BOLD, "  ");
        print_color(ANSI_WHITE ANSI_BOLD, commands[i].name);
        print_color(ANSI_WHITE, " - ");
        print_color(ANSI_CYAN, commands[i].desc);
        println("");
    }
    println("");
    print_color(ANSI_YELLOW, "Dica: Use 'info' para ver informacoes detalhadas do sistema\n");
    
    return 0;
}