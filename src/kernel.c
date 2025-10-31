// Kernel 64-bit - I/O serial

// Tipos básicos usando built-ins do GCC
typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

// Definições de portas
#define COM1 0x3F8
#define BUFFER_SIZE 50

// Funções de I/O inline
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

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

// Limpa buffer
void clear_buffer(char* buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = 0;
    }
}

// Kernel principal
void kernel_main(uint64_t mb_info) {
    // Inicializar serial para output
    if (serial_init() != 0) {
        // Serial falhou, mas não podemos nem avisar. Halt.
        while(1) __asm__ volatile("hlt");
    }
    /* Inicializar subsistema de memória (recebe ponteiro multiboot) */
    extern void mem_init(uint64_t mb_info);
    mem_init(mb_info);
    
    // Banner do kernel
    println("");
    println("╔══════════════════════════════════════════╗");
    println("║                KERNEL64                 ║");
    println("║           Sistema Operacional           ║");
    println("╚══════════════════════════════════════════╝");
    println("");
    println("Status: Operacional");
    println("Arquitetura: x86_64");
    println("Processador: Intel");
    println("");
    print("Digite algo (max 50 chars): ");
    
    // Buffer para input com proteção
    char input[BUFFER_SIZE + 1];
    int index = 0;
    clear_buffer(input, BUFFER_SIZE + 1);
    
    // Loop principal de input
    while (1) {
        char c = serial_getchar();
        
        // Backspace ou Delete
        if (c == 0x08 || c == 0x7F) {
            if (index > 0) {
                index--;
                input[index] = '\0';
                print("\b \b"); // Apagar caractere visualmente
            }
        }
        // Enter ou Line Feed
        else if (c == '\r' || c == '\n') {
            input[index] = '\0';
            print("\r\nVoce digitou: ");
            print(input);
            print("\r\n\r\nDigite algo: ");
            clear_buffer(input, BUFFER_SIZE + 1);
            index = 0;
        }
        // Caractere imprimível
        else if (c >= 32 && c <= 126) {
            if (index < BUFFER_SIZE) {
                input[index++] = c;
                serial_putchar(c); // Echo
            }
            // Buffer cheio - ignorar silenciosamente
        }
        // Caracteres de controle ignorados
    }
    
    // Never reached
    while(1) __asm__ volatile("hlt");
}