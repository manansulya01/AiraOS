#include <stdint.h>

#include "interrupts.h"
#include "terminal.h"
#include "shell.h"
#include "memory.h"

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

extern void gdt_init(void);

extern void pic_remap(void);
extern void pic_unmask_irq(uint8_t irq);
extern void pit_init(uint32_t frequency);
extern void keyboard_init(void);

static struct idt_entry idt[256];
static struct idt_ptr idtp;

static void idt_set_gate(int vector, void (*handler)(void))
{
    uint64_t address = (uint64_t)handler;

    idt[vector].offset_low   = address & 0xFFFF;
    idt[vector].selector     = 0x08;
    idt[vector].ist         = 0;
    idt[vector].type_attr   = 0x8E;
    idt[vector].offset_mid  = (address >> 16) & 0xFFFF;
    idt[vector].offset_high = (address >> 32) & 0xFFFFFFFF;
    idt[vector].zero        = 0;
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

void kernel_main64(uint64_t magic, uint64_t multiboot_info)
{
    (void)magic;
    (void)multiboot_info;

    terminal_init();

    terminal_write("========================================\n");
    terminal_write("              AiraOS v0.4\n");
    terminal_write("        Interactive Kernel Shell\n");
    terminal_write("========================================\n\n");

    terminal_write("Initializing GDT... ");
    gdt_init();
    terminal_write("OK\n");

    terminal_write("Initializing IDT... ");
    idt_init();
    terminal_write("OK\n");

    terminal_write("Initializing PIC... ");
    pic_remap();
    terminal_write("OK\n");

    terminal_write("Initializing PIT... ");
    pit_init(100);
    terminal_write("100 Hz\n");

    terminal_write("Initializing memory... ");
    memory_init(multiboot_info);
    terminal_write("OK\n");

    terminal_write("Initializing keyboard... ");
    keyboard_init();
    terminal_write("IRQ1 ONLINE\n");

    pic_unmask_irq(0);
    pic_unmask_irq(1);

    terminal_write("Hardware interrupts: ENABLED\n\n");

    shell_init();

    __asm__ volatile ("sti");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
