#include <stdint.h>
#include "interrupts.h"

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static void clear_screen(void)
{
    for (int i = 0; i < 80 * 25; i++)
        VGA[i] = ((uint16_t)0x07 << 8) | ' ';
}

static void print(const char *text, int row)
{
    for (int col = 0; text[col] != '\0'; col++)
        VGA[row * 80 + col] =
            ((uint16_t)0x0F << 8) | (uint8_t)text[col];
}

static void print_hex(uint64_t value, int row)
{
    const char *hex = "0123456789ABCDEF";

    print("0x", row);

    for (int i = 0; i < 16; i++) {
        VGA[row * 80 + 2 + i] =
            ((uint16_t)0x0F << 8) |
            (uint8_t)hex[(value >> ((15 - i) * 4)) & 0xF];
    }
}

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

extern void idt_load(struct idt_ptr *ptr);

extern void isr_default(void);
extern void isr0(void);
extern void isr3(void);
extern void isr6(void);
extern void isr13(void);
extern void isr14(void);
extern void irq0(void);
extern void irq1(void);

static struct idt_entry idt[256];
static struct idt_ptr idtp;

static void idt_set_gate(int vector, void (*handler)(void))
{
    uint64_t address = (uint64_t)handler;

    idt[vector].offset_low  = address & 0xFFFF;
    idt[vector].selector    = 0x08;
    idt[vector].ist        = 0;
    idt[vector].type_attr  = 0x8E;
    idt[vector].offset_mid = (address >> 16) & 0xFFFF;
    idt[vector].offset_high = (address >> 32) & 0xFFFFFFFF;
    idt[vector].zero = 0;
}

void idt_init(void)
{
    for (int i = 0; i < 256; i++)
        idt_set_gate(i, isr_default);

    idt_set_gate(0, isr0);
    idt_set_gate(3, isr3);
    idt_set_gate(6, isr6);
    idt_set_gate(13, isr13);
    idt_set_gate(14, isr14);
    idt_set_gate(32, irq0);
    idt_set_gate(33, irq1);

    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint64_t)&idt;

    idt_load(&idtp);
}

extern void pic_remap(void);
extern void pic_unmask_irq(uint8_t irq);
extern void pit_init(uint32_t frequency);
extern void keyboard_init(void);

void kernel_main64(uint64_t magic, uint64_t multiboot_info)
{
    (void)magic;
    (void)multiboot_info;

    clear_screen();

    print("AiraOS", 6);
    print("x86_64 long mode active.", 8);
    print("Initializing GDT...", 10);

    gdt_init();

    print("GDT: OK", 11);
    print("Initializing IDT...", 12);

    idt_init();

    print("IDT: OK", 13);
    print("CPU exception infrastructure: ONLINE", 15);
    print("Initializing PIC...", 17);

    pic_remap();
    print("PIC: OK", 18);

    print("Initializing PIT...", 19);

    pit_init(100);
    print("PIT: 100 Hz", 20);

    keyboard_init();
    pic_unmask_irq(0);
    pic_unmask_irq(1);

    print("Keyboard IRQ1: ONLINE", 21);
    print("Hardware interrupts: ENABLED", 22);

    __asm__ volatile ("sti");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
