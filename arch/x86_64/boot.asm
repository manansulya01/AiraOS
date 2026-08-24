bits 32

section .multiboot2
align 8

header_start:
    dd 0xE85250D6
    dd 0
    dd header_end - header_start
    dd -(0xE85250D6 + 0 + (header_end - header_start))

    dw 0
    dw 0
    dd 8

header_end:

section .text
global _start
extern kernel_main

_start:
    cli

    mov esp, stack_top

    push ebx
    push eax

    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16

stack_bottom:
    resb 16384
stack_top:
