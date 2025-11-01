/* terminal.c - interface de terminal do kernel */
#include "terminal.h"
#include "colors.h"

/* Funções externas do kernel */
extern void print(const char* str);
extern void println(const char* str);
extern char serial_getchar(void);
extern void serial_putchar(char c);
extern int execute_command(const char *cmd_line);

#define BUFFER_SIZE 50

/* Limpa buffer */
static void clear_buffer(char* buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = 0;
    }
}

/* Imprime com cor */
static void print_color(const char* color, const char* text) {
    print(color);
    print(text);
    print(ANSI_RESET);
}

void terminal_init(void) {
    /* Banner colorido e moderno */
    print_color(ANSI_CYAN ANSI_BOLD, "╔══════════════════════════════════════════╗\n");
    print_color(ANSI_CYAN ANSI_BOLD, "║");
    print_color(ANSI_YELLOW ANSI_BOLD, "              KERNEL64 OS");
    print_color(ANSI_CYAN ANSI_BOLD, "              ║\n");
    print_color(ANSI_CYAN ANSI_BOLD, "╚══════════════════════════════════════════╝\n");
    println("");
    
    print_color(ANSI_GREEN, "[OK] Sistema inicializado\n");
    print_color(ANSI_BLUE, "[INFO] Digite 'help' para comandos\n");
    print_color(ANSI_BLUE, "[INFO] Digite 'info' para detalhes\n");
    println("");
}

void terminal_run(void) {
    print_color(ANSI_GREEN ANSI_BOLD, "kernel");
    print_color(ANSI_WHITE, "$ ");
    
    /* Buffer para input */
    char input[BUFFER_SIZE + 1];
    int index = 0;
    clear_buffer(input, BUFFER_SIZE + 1);
    
    /* Loop principal de input */
    while (1) {
        char c = serial_getchar();
        
        /* Backspace ou Delete */
        if (c == 0x08 || c == 0x7F) {
            if (index > 0) {
                index--;
                input[index] = '\0';
                print("\b \b");
            }
        }
        /* Enter ou Line Feed */
        else if (c == '\r' || c == '\n') {
            input[index] = '\0';
            print("\r\n");
            
            /* Executar comando */
            int result = execute_command(input);
            
            /* Feedback visual baseado no resultado */
            if (result == 0) {
                print_color(ANSI_GREEN, "[OK] ");
            } else if (result == -1) {
                print_color(ANSI_YELLOW, "[WARN] ");
            } else {
                print_color(ANSI_RED, "[ERROR] ");
            }
            println("Comando executado");
            
            clear_buffer(input, BUFFER_SIZE + 1);
            index = 0;
            
            /* Novo prompt */
            print_color(ANSI_GREEN ANSI_BOLD, "kernel");
            print_color(ANSI_WHITE, "$ ");
        }
        /* Caractere imprimível */
        else if (c >= 32 && c <= 126) {
            if (index < BUFFER_SIZE) {
                input[index++] = c;
                serial_putchar(c);
            } else {
                /* Buffer cheio - beep visual */
                print_color(ANSI_RED, "\a");
            }
        }
    }
}
