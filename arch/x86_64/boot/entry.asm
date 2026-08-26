bits 32

CODE64 equ 0x08
DATA64 equ 0x10

section .multiboot2
align 8

header_start:
    dd 0xE85250D6
    dd 0
    dd header_end - header_start
    dd -(0xE85250D6 + (header_end - header_start))

    dw 0
    dw 0
    dd 8

header_end:

section .text
global _start
extern kernel_main64

_start:
    cli

    mov esp, stack_top
    and esp, 0xFFFFFFF0

    ; Multiboot2 arguments supplied by GRUB.
    mov [multiboot_magic], eax
    mov [multiboot_info], ebx

    ; Load 32-bit compatible GDT.
    lgdt [gdt64_descriptor]

    ; Enable PAE.
    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax

    ; Load PML4.
    mov eax, pml4_table
    mov cr3, eax

    ; Enable long mode.
    mov ecx, 0xC0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr

    ; Enable paging.
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax

    ; Enter 64-bit code.
    jmp CODE64:long_mode_start

bits 64

long_mode_start:
    mov ax, DATA64
    mov ds, ax
    mov es, ax
    mov ss, ax

    xor ebp, ebp

    ; SysV x86_64 calling convention:
    ; RDI = magic
    ; RSI = Multiboot information address
    mov edi, [multiboot_magic]
    mov esi, [multiboot_info]

    call kernel_main64

.hang:
    cli
    hlt
    jmp .hang


section .data

align 8

multiboot_magic:
    dd 0

multiboot_info:
    dd 0


align 8

gdt64:
    dq 0x0000000000000000
    dq 0x00209A0000000000
    dq 0x0000920000000000

; 32-bit LGDT descriptor:
; 16-bit limit + 32-bit base.
gdt64_descriptor:
    dw gdt64_descriptor_end - gdt64 - 1
    dd gdt64
gdt64_descriptor_end:


; Page tables are initialized, therefore they belong in .data,
; not .bss.

align 4096

pml4_table:
    dq pdpt_table + 0x03
    times 511 dq 0

align 4096

pdpt_table:
    dq pd_table + 0x03
    times 511 dq 0

align 4096

pd_table:
    ; Identity-map the first 1 GiB using 2 MiB pages.
    ;
    ; bit 0 = Present
    ; bit 1 = Writable
    ; bit 7 = Page Size (2 MiB)

%assign i 0
%rep 512
    dq (i * 0x200000) | 0x83
%assign i i + 1
%endrep


section .bss

align 16

stack_bottom:
    resb 16384

stack_top:


section .note.GNU-stack
