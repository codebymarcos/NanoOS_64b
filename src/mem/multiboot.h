/* multiboot.h - stub para inicialização de memória */
#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stddef.h>
#include <stdint.h>

/* Inicializa o subsistema de memória (recebe ponteiro Multiboot2) */
void mem_init(uint64_t mb_info);

/* Utilitários de impressão usados por outros módulos */
void print_hex_u64(uint64_t v);

#endif /* MULTIBOOT_H */
