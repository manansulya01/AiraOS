#ifndef AIRAOS_PIC_H
#define AIRAOS_PIC_H

#include <stdint.h>

void pic_remap(void);
void pic_eoi(uint8_t irq);
void pic_unmask_irq(uint8_t irq);

#endif
