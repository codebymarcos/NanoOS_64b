/* readsector.c - comando para ler setor do disco */
#include "commands.h"

extern void println(const char* str);
extern void print(const char* str);
extern void print_hex_u64(uint64_t v);
extern int ata_read_sector(uint32_t sector, uint8_t *buffer);

/* Buffer para armazenar setor lido (512 bytes) */
static uint8_t sector_buffer[512];

/* Helper: converte string decimal simples para int */
static int simple_atoi(const char *str) {
    int result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

int cmd_readsector(int argc, char *argv[]) {
    if (argc < 2) {
        println("Uso: readsector <numero_setor>");
        println("Exemplo: readsector 0");
        return -1;
    }

    uint32_t sector = (uint32_t)simple_atoi(argv[1]);
    
    print("Lendo setor ");
    print_hex_u64(sector);
    println("...");
    
    if (ata_read_sector(sector, sector_buffer) < 0) {
        println("Erro ao ler setor");
        return -1;
    }
    
    println("Setor lido com sucesso. Primeiros 64 bytes:");
    
    /* Imprimir primeiros 64 bytes em hex (4 linhas de 16 bytes) */
    for (int line = 0; line < 4; line++) {
        for (int col = 0; col < 16; col++) {
            uint8_t byte = sector_buffer[line * 16 + col];
            /* Imprimir byte em hex (2 dígitos) */
            char hex[4];
            hex[0] = (byte >> 4) < 10 ? ('0' + (byte >> 4)) : ('a' + (byte >> 4) - 10);
            hex[1] = (byte & 0xF) < 10 ? ('0' + (byte & 0xF)) : ('a' + (byte & 0xF) - 10);
            hex[2] = ' ';
            hex[3] = '\0';
            print(hex);
        }
        println("");
    }
    
    return 0;
}