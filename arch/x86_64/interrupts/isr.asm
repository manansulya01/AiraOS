bits 64

section .text

extern exception_handler

global isr_default
global isr0
global isr3
global isr6
global isr13
global isr14

isr_default:
    push 0
    push 255
    jmp isr_common

; Exceptions WITHOUT CPU-pushed error code
isr0:
    push 0
    push 0
    jmp isr_common

isr3:
    push 0
    push 3
    jmp isr_common

isr6:
    push 0
    push 6
    jmp isr_common

; Exceptions WITH CPU-pushed error code
isr13:
    push 13
    jmp isr_common

isr14:
    push 14
    jmp isr_common

isr_common:
    cld

    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax

    mov rdi, rsp
    call exception_handler

    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    add rsp, 16
    iretq
