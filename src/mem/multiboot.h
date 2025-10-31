/* multiboot.h - stub para inicialização de memória */
#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stddef.h>
#include <stdint.h>

/* Inicializa o subsistema de memória (recebe ponteiro Multiboot2) */
void mem_init(uint64_t mb_info);

#endif /* MULTIBOOT_H */
