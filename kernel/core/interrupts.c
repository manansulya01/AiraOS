#include <stdint.h>

#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "shell.h"
#include "scheduler.h"

uint64_t irq_handler(uint64_t *stack)
{
    uint64_t vector = stack[15];

    if (vector == 32) {
        pit_tick();

        uint64_t next_rsp =
            scheduler_tick((uint64_t)stack);

        pic_eoi(0);

        return next_rsp;
    }

    if (vector == 33) {
        keyboard_irq();

        char c = keyboard_scancode_to_ascii(
            keyboard_last_scancode()
        );

        if (c)
            shell_handle_char(c);

        pic_eoi(1);
    }

    return (uint64_t)stack;
}
