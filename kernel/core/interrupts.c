#include <stdint.h>

#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "shell.h"

void irq_handler(uint64_t *stack)
{
    uint64_t vector = stack[15];

    if (vector == 32) {
        pit_tick();
        pic_eoi(0);
    }
    else if (vector == 33) {
        keyboard_irq();

        char c = keyboard_scancode_to_ascii(
            keyboard_last_scancode()
        );

        if (c)
            shell_handle_char(c);

        pic_eoi(1);
    }
}
