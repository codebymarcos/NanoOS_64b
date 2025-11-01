/* info.c - comando para mostrar informações do sistema */
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

int cmd_info(int argc, char* argv[]) {
    (void)argc; (void)argv; /* não usados */
    
    print_color(ANSI_CYAN ANSI_BOLD, "╔══════════════════════════════════════════╗\n");
    print_color(ANSI_CYAN ANSI_BOLD, "║");
    print_color(ANSI_YELLOW ANSI_BOLD, "           INFORMAÇÕES DO SISTEMA");
    print_color(ANSI_CYAN ANSI_BOLD, "           ║\n");
    print_color(ANSI_CYAN ANSI_BOLD, "╚══════════════════════════════════════════╝\n");
    println("");
    
    /* Informações básicas */
    print_color(ANSI_GREEN ANSI_BOLD, "● Sistema: ");
    print_color(ANSI_WHITE, "Kernel64 OS\n");
    print_color(ANSI_GREEN ANSI_BOLD, "● Arquitetura: ");
    print_color(ANSI_WHITE, "x86_64\n");
    print_color(ANSI_GREEN ANSI_BOLD, "● Status: ");
    print_color(ANSI_GREEN, "Operacional ✓\n");
    println("");
    
    /* Informações de memória */
    print_color(ANSI_BLUE ANSI_BOLD, "╔══════════════════════════════════════════╗\n");
    print_color(ANSI_BLUE ANSI_BOLD, "║");
    print_color(ANSI_CYAN ANSI_BOLD, "               MEMÓRIA RAM");
    print_color(ANSI_BLUE ANSI_BOLD, "               ║\n");
    print_color(ANSI_BLUE ANSI_BOLD, "╚══════════════════════════════════════════╝\n");
    
    extern uint64_t phys_get_total_pages(void);
    extern uint64_t phys_get_free_pages(void);
    
    uint64_t total_pages = phys_get_total_pages();
    uint64_t free_pages = phys_get_free_pages();
    uint64_t used_pages = total_pages - free_pages;
    
    print_color(ANSI_YELLOW, "  Total: ");
    print_color(ANSI_WHITE ANSI_BOLD, "");
    extern void print_hex_u64(uint64_t v);
    print_hex_u64(total_pages * 4096 / 1024 / 1024);
    print_color(ANSI_WHITE, " MB\n");
    
    print_color(ANSI_YELLOW, "  Livre: ");
    print_color(ANSI_GREEN ANSI_BOLD, "");
    print_hex_u64(free_pages * 4096 / 1024 / 1024);
    print_color(ANSI_WHITE, " MB\n");
    
    print_color(ANSI_YELLOW, "  Usada: ");
    print_color(ANSI_RED ANSI_BOLD, "");
    print_hex_u64(used_pages * 4096 / 1024 / 1024);
    print_color(ANSI_WHITE, " MB\n");
    println("");
    
    /* Informações de disco */
    print_color(ANSI_MAGENTA ANSI_BOLD, "╔══════════════════════════════════════════╗\n");
    print_color(ANSI_MAGENTA ANSI_BOLD, "║");
    print_color(ANSI_CYAN ANSI_BOLD, "                DISCO");
    print_color(ANSI_MAGENTA ANSI_BOLD, "                    ║\n");
    print_color(ANSI_MAGENTA ANSI_BOLD, "╚══════════════════════════════════════════╝\n");
    
    print_color(ANSI_YELLOW, "  Driver ATA/IDE: ");
    print_color(ANSI_GREEN, "Inicializado ✓\n");
    print_color(ANSI_YELLOW, "  Interface: ");
    print_color(ANSI_WHITE, "IDE (Primary Master)\n");
    print_color(ANSI_YELLOW, "  Protocolo: ");
    print_color(ANSI_WHITE, "LBA28\n");
    println("");
    
    return 0;
}