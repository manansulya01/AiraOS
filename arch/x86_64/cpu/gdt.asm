bits 64

section .text
global gdt_init

gdt_init:
    lgdt [gdt_descriptor]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    xor ax, ax
    mov fs, ax
    mov gs, ax

    push 0x08
    lea rax, [rel .reload_cs]
    push rax
    retfq

.reload_cs:
    ret

section .data
align 8

gdt:
    dq 0x0000000000000000
    dq 0x00209A0000000000
    dq 0x0000920000000000

gdt_descriptor:
    dw gdt_descriptor_end - gdt - 1
    dq gdt

gdt_descriptor_end:
