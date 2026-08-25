bits 64

section .text

global io_outb
global io_inb

io_outb:
    mov dx, di
    mov al, sil
    out dx, al
    ret

io_inb:
    mov dx, di
    in al, dx
    movzx eax, al
    ret
