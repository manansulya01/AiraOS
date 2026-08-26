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
global isr1
global isr2
global isr4
global isr5
global isr7
global isr8
global isr10
global isr11
global isr12
global isr16
global isr17
global isr18
global isr19
global isr20
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

isr1:
    push 0
    push 1
    jmp exception_common

isr2:
    push 0
    push 2
    jmp exception_common

isr4:
    push 0
    push 4
    jmp exception_common

isr5:
    push 0
    push 5
    jmp exception_common

isr7:
    push 0
    push 7
    jmp exception_common

isr8:
    push 0
    push 8
    jmp exception_common

isr10:
    push 10
    jmp exception_common

isr11:
    push 11
    jmp exception_common

isr12:
    push 12
    jmp exception_common

isr16:
    push 0
    push 16
    jmp exception_common

isr17:
    push 17
    jmp exception_common

isr18:
    push 0
    push 18
    jmp exception_common

isr19:
    push 0
    push 19
    jmp exception_common

isr20:
    push 0
    push 20
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

    ; irq_handler returns the interrupt-frame RSP
    ; that should be restored.
    mov rsp, rax

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
