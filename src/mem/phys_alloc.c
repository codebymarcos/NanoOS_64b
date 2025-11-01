/* phys_alloc.c - alocador físico simples (bitmap)
 * Usa a maior região livre fornecida pelo Multiboot para montar um bitmap
 * e gerir páginas de 4KiB. Implementação intencionalmente simples para testes.
 */

#include "phys_alloc.h"
#include <stdint.h>

extern void println(const char* str);
extern void print(const char* str);
extern void print_hex_u64(uint64_t v);

static uint8_t *bitmap = 0; /* ponteiro para map de bits */
static uint64_t base = 0;   /* primeira página gerida (paddr) */
static uint64_t total_pages = 0;
static uint64_t bitmap_bytes = 0;

static inline void set_bit(uint64_t idx) {
    bitmap[idx >> 3] |= (1 << (idx & 7));
}
static inline void clear_bit(uint64_t idx) {
    bitmap[idx >> 3] &= ~(1 << (idx & 7));
}
static inline int test_bit(uint64_t idx) {
    return (bitmap[idx >> 3] >> (idx & 7)) & 1;
}

void phys_init(uint64_t start, uint64_t end) {
    if (end <= start) {
        println("phys_init: região inválida");
        return;
    }

    uint64_t full_pages = (end - start) / 4096;
    if (full_pages == 0) {
        println("phys_init: nenhuma página disponível");
        return;
    }

    /* Precisamos reservar espaço para o bitmap dentro da região. Iteramos até estabilizar */
    uint64_t bpages = 0;
    uint64_t managed = full_pages;
    for (int iter = 0; iter < 4; iter++) {
        uint64_t bbytes = (managed + 7) / 8;
        uint64_t nbpages = (bbytes + 4095) / 4096;
        if (nbpages == bpages) break;
        bpages = nbpages;
        managed = full_pages - bpages;
    }

    bitmap_bytes = (managed + 7) / 8;
    bitmap = (uint8_t*) (uintptr_t) start;
    base = start + ( (bitmap_bytes + 4095) / 4096 ) * 4096;
    total_pages = managed;

    /* limpar bitmap */
    for (uint64_t i = 0; i < bitmap_bytes; i++) bitmap[i] = 0;

    println("phys_init: inicializado");
    print(" phys base="); print_hex_u64(base);
    print(" pages="); print_hex_u64(total_pages);
    println("");
}

uint64_t phys_alloc_page(void) {
    if (!bitmap || total_pages == 0) return 0;
    for (uint64_t byte = 0; byte < bitmap_bytes; byte++) {
        if (bitmap[byte] != 0xFF) {
            for (int bit = 0; bit < 8; bit++) {
                uint64_t idx = byte * 8 + bit;
                if (idx >= total_pages) return 0;
                if (!test_bit(idx)) {
                    set_bit(idx);
                    return base + idx * 4096;
                }
            }
        }
    }
    return 0; /* sem memória */
}

void phys_free_page(uint64_t paddr) {
    if (!bitmap || total_pages == 0) return;
    if (paddr < base) return;
    uint64_t idx = (paddr - base) / 4096;
    if (idx >= total_pages) return;
    clear_bit(idx);
}
