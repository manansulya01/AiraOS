#include <stdint.h>
#include "io.h"

#define KEYBOARD_DATA 0x60

static volatile uint8_t last_scancode = 0;

void keyboard_init(void)
{
    last_scancode = 0;
}

void keyboard_irq(void)
{
    last_scancode = io_inb(KEYBOARD_DATA);
}

uint8_t keyboard_last_scancode(void)
{
    return last_scancode;
}
