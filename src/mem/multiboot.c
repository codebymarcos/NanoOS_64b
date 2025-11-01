
/* multiboot.c - stub mínimo
 * Será estendido para analisar Multiboot2 e montar o mapa físico.
 * Por enquanto apenas imprime uma mensagem via serial.
 */

#include "multiboot.h"

/* funções de print definidas em kernel.c */
extern void println(const char* str);
extern void print(const char* str);
extern void serial_putchar(char c);

/* helper simples: imprime uint64 em hex (sem dependências) */
void print_hex_u64(uint64_t v) {
    char buf[19]; /* 0x + 16 hex + \0 */
    buf[0] = '0'; buf[1] = 'x';
    for (int i = 0; i < 16; i++) {
        int shift = (15 - i) * 4;
        uint8_t nib = (v >> shift) & 0xF;
        buf[2 + i] = (nib < 10) ? ('0' + nib) : ('a' + (nib - 10));
    }
    buf[18] = '\0';
    print(buf);
}

void mem_init(uint64_t mb_info) {
    if (!mb_info) {
        return;
    }

    uint8_t *base = (uint8_t*)(uintptr_t)mb_info;
    uint32_t total_size = *(uint32_t*)(base + 0);
    /* reserved at offset 4 */

    uint32_t off = 8; /* tags start after total_size + reserved */
    uint64_t best_addr = 0, best_len = 0;
    while (off + 8 <= total_size) {
        uint32_t tag_type = *(uint32_t*)(base + off);
        uint32_t tag_size = *(uint32_t*)(base + off + 4);
        if (tag_type == 0) break; /* end tag */

        if (tag_type == 6) { /* MEMORY_MAP tag */
            uint32_t entry_size = *(uint32_t*)(base + off + 8);
            /* uint32_t entry_version = *(uint32_t*)(base + off + 12); */
            uint32_t ent_off = off + 16;
            while (ent_off + entry_size <= off + tag_size) {
                uint64_t addr = *(uint64_t*)(base + ent_off + 0);
                uint64_t len  = *(uint64_t*)(base + ent_off + 8);
                uint32_t type = *(uint32_t*)(base + ent_off + 16);

                /* track biggest available region (type 1) for allocator */
                if (type == 1 && len > best_len) {
                    best_len = len;
                    best_addr = addr;
                }

                ent_off += entry_size;
            }
        }

        /* advance to next tag (8-byte aligned) */
        uint32_t adv = ((tag_size + 7) & ~7);
        off += adv;
    }

    if (best_len > 4096) {
        extern void phys_init(uint64_t start, uint64_t end);
        phys_init(best_addr, best_addr + best_len);
    }
}
