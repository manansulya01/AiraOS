#include <stdint.h>
#include "io.h"

#define PIC1        0x20
#define PIC2        0xA0
#define PIC1_DATA   0x21
#define PIC2_DATA   0xA1

#define ICW1_INIT   0x10
#define ICW1_ICW4   0x01
#define ICW4_8086   0x01
#define PIC_EOI     0x20

static void io_wait(void)
{
    io_outb(0x80, 0);
}

void pic_remap(void)
{
    uint8_t mask1 = io_inb(PIC1_DATA);
    uint8_t mask2 = io_inb(PIC2_DATA);

    io_outb(PIC1, ICW1_INIT | ICW1_ICW4);
    io_wait();
    io_outb(PIC2, ICW1_INIT | ICW1_ICW4);
    io_wait();

    io_outb(PIC1_DATA, 0x20);
    io_wait();
    io_outb(PIC2_DATA, 0x28);
    io_wait();

    io_outb(PIC1_DATA, 4);
    io_wait();
    io_outb(PIC2_DATA, 2);
    io_wait();

    io_outb(PIC1_DATA, ICW4_8086);
    io_wait();
    io_outb(PIC2_DATA, ICW4_8086);
    io_wait();

    io_outb(PIC1_DATA, mask1);
    io_outb(PIC2_DATA, mask2);
}

void pic_eoi(uint8_t irq)
{
    if (irq >= 8)
        io_outb(PIC2, PIC_EOI);

    io_outb(PIC1, PIC_EOI);
}

void pic_unmask_irq(uint8_t irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
        port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = io_inb(port);
    value &= ~(1 << irq);
    io_outb(port, value);
}
