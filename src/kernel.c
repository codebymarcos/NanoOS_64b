// Kernel 64-bit - I/O serial

#include "io.h"

// Tipos básicos usando built-ins do GCC
typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

// Definições de portas
#define COM1 0x3F8

// Inicializa serial COM1
int serial_init(void) {
    outb(COM1 + 1, 0x00);    // desabilita IRQs
    outb(COM1 + 3, 0x80);    // habilita DLAB (baud)
    outb(COM1 + 0, 0x03);    // divisor lo (38400)
    outb(COM1 + 1, 0x00);    // divisor hi
    outb(COM1 + 3, 0x03);    // 8N1
    outb(COM1 + 2, 0xC7);    // enable FIFO
    outb(COM1 + 4, 0x0B);    // IRQs e RTS/DSR
    outb(COM1 + 4, 0x1E);    // loopback para teste
    outb(COM1 + 0, 0xAE);    // enviar byte teste
    
    // verifica retorno do teste
    if(inb(COM1 + 0) != 0xAE) {
        return 1; // serial falhou
    }

    // modo normal
    outb(COM1 + 4, 0x0F);
    return 0; // Serial OK
}

// Checa se pode transmitir
static inline int serial_transmit_ready(void) {
    return inb(COM1 + 5) & 0x20;
}

// Checa se há dado disponível
static inline int serial_received(void) {
    return inb(COM1 + 5) & 0x01;
}

// Envia caractere (com timeout)
void serial_putchar(char c) {
    uint32_t timeout = 100000;
    while (!serial_transmit_ready() && --timeout);
    if (timeout) outb(COM1, c);
}

// Lê caractere da serial (bloqueante)
char serial_getchar(void) {
    while (!serial_received());
    return inb(COM1);
}

// Imprime string
void print(const char* str) {
    if (!str) return;
    while (*str) {
        serial_putchar(*str++);
    }
}

// Imprime string com newline
void println(const char* str) {
    print(str);
    print("\r\n");
}

// Kernel principal
void kernel_main(uint64_t mb_info) {
    /* Inicializar serial para output */
    if (serial_init() != 0) {
        while(1) __asm__ volatile("hlt");
    }
    
    /* Inicializar subsistema de memória */
    extern void mem_init(uint64_t mb_info);
    mem_init(mb_info);
    
    /* Inicializar driver ATA */
    extern void ata_init(void);
    ata_init();
    
    /* Inicializar e executar terminal */
    extern void terminal_init(void);
    extern void terminal_run(void);
    
    terminal_init();
    terminal_run();
    
    /* Nunca deve chegar aqui */
    while(1) __asm__ volatile("hlt");
}