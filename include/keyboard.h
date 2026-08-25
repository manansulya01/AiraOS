#ifndef AIRAOS_KEYBOARD_H
#define AIRAOS_KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
void keyboard_irq(void);
uint8_t keyboard_last_scancode(void);

#endif
