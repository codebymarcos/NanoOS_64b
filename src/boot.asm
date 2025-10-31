; Bootloader 64-bit com Multiboot Header
section .multiboot_header
header_start:
    dd 0xe85250d6                ; magic number (multiboot 2)
    dd 0                         ; architecture 0 (protected mode i386)
    dd header_end - header_start ; header length
    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))
    
    ; end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .text
bits 32
global _start

_start:
    ; Setup stack
    mov esp, stack_top
    
    ; Checar multiboot
    call check_multiboot
    
    ; Configurar paginação para 64-bit
    call setup_paging
    
    ; Entrar em 64-bit
    lgdt [gdt64.pointer]
    jmp gdt64.code:long_mode_start

check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    jmp error

setup_paging:
    ; Mapear primeira página
    mov eax, p3_table
    or eax, 0b11
    mov [p4_table], eax
    
    mov eax, p2_table
    or eax, 0b11
    mov [p3_table], eax
    
    ; Mapear cada entrada P2 para 2MB pages
    mov ecx, 0
.map_p2_table:
    mov eax, 0x200000
    mul ecx
    or eax, 0b10000011
    mov [p2_table + ecx * 8], eax
    
    inc ecx
    cmp ecx, 512
    jne .map_p2_table
    
    ; Habilitar PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    
    ; Setar CR3
    mov eax, p4_table
    mov cr3, eax
    
    ; Habilitar long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr
    
    ; Habilitar paginação
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    
    ret

error:
    ; Error simples em 32-bit
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al
    hlt

section .bss
align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096
stack_bottom:
    resb 4096 * 4
stack_top:

section .rodata
gdt64:
    dq 0
.code: equ $ - gdt64
    dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .text
bits 64
long_mode_start:
    ; Limpar segmentos
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Chamar kernel C, passando ponteiro multiboot (em RBX) no primeiro argumento (RDI)
    extern kernel_main
    mov rdi, rbx
    call kernel_main
    
    ; Halt se retornar
.halt:
    hlt
    jmp .halt