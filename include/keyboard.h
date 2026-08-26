#ifndef AIRAOS_KEYBOARD_H
#define AIRAOS_KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
void keyboard_irq(void);

uint8_t keyboard_last_scancode(void);
char keyboard_scancode_to_ascii(uint8_t scancode);

/* Scheduler-friendly keyboard input queue. */
char keyboard_get_char(void);

#endif
