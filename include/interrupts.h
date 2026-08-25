#ifndef AIRAOS_INTERRUPTS_H
#define AIRAOS_INTERRUPTS_H

#include <stdint.h>

void gdt_init(void);
void idt_init(void);
void exception_handler(uint64_t *stack);

#endif
