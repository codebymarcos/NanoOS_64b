/* mem.c - comando mem */
#include "commands.h"
#include "colors.h"

extern void println(const char* str);
extern void print(const char* str);
extern void print_hex_u64(uint64_t v);

extern uint64_t phys_get_total_pages(void);
extern uint64_t phys_get_free_pages(void);

/* Imprime com cor */
static void print_color(const char* color, const char* text) {
    print(color);
    print(text);
    print(ANSI_RESET);
}

int cmd_mem(int argc, char *argv[]) {
    (void)argc; (void)argv;

    uint64_t total = phys_get_total_pages();
    uint64_t free = phys_get_free_pages();
    uint64_t used = total - free;

        print_color(ANSI_BLUE ANSI_BOLD, "══════════════════════════════════════════\n");
        print_color(ANSI_CYAN ANSI_BOLD, "            STATUS DA MEMÓRIA            \n");
        print_color(ANSI_BLUE ANSI_BOLD, "══════════════════════════════════════════\n");
        println("");
    
    /* Páginas */
    print_color(ANSI_YELLOW, "Paginas (4KiB):\n");
    print_color(ANSI_WHITE, "  Total: ");
    print_color(ANSI_CYAN ANSI_BOLD, "");
    print_hex_u64(total);
    print_color(ANSI_WHITE, " paginas\n");
    
    print_color(ANSI_WHITE, "  Livres: ");
    print_color(ANSI_GREEN ANSI_BOLD, "");
    print_hex_u64(free);
    print_color(ANSI_WHITE, " paginas\n");
    
    print_color(ANSI_WHITE, "  Usadas: ");
    print_color(ANSI_RED ANSI_BOLD, "");
    print_hex_u64(used);
    print_color(ANSI_WHITE, " paginas\n");
    println("");

    /* Calcular em MB aproximado */
    uint64_t total_mb = (total * 4096) / (1024 * 1024);
    uint64_t free_mb = (free * 4096) / (1024 * 1024);
    uint64_t used_mb = (used * 4096) / (1024 * 1024);

    print_color(ANSI_YELLOW, "Memoria RAM:\n");
    print_color(ANSI_WHITE, "  Total: ");
    print_color(ANSI_CYAN ANSI_BOLD, "");
    print_hex_u64(total_mb);
    print_color(ANSI_WHITE, " MB\n");
    
    print_color(ANSI_WHITE, "  Livre: ");
    print_color(ANSI_GREEN ANSI_BOLD, "");
    print_hex_u64(free_mb);
    print_color(ANSI_WHITE, " MB\n");
    
    print_color(ANSI_WHITE, "  Usada: ");
    print_color(ANSI_RED ANSI_BOLD, "");
    print_hex_u64(used_mb);
    print_color(ANSI_WHITE, " MB\n");

    return 0;
}