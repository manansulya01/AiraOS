bits 64

section .text

extern exception_handler
extern irq_handler

global isr_default
global isr0
global isr3
global isr6
global isr13
global isr14
global irq0
global irq1

isr_default:
    push 0
    push 255
    jmp exception_common

isr0:
    push 0
    push 0
    jmp exception_common

isr3:
    push 0
    push 3
    jmp exception_common

isr6:
    push 0
    push 6
    jmp exception_common

isr13:
    push 13
    jmp exception_common

isr14:
    push 14
    jmp exception_common

irq0:
    push 0
    push 32
    jmp irq_common

irq1:
    push 0
    push 33
    jmp irq_common

exception_common:
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

irq_common:
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
    call irq_handler

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
