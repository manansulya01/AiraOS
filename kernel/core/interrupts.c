#include <stdint.h>
#include "pic.h"
#include "pit.h"
#include "keyboard.h"

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static void print_hex8(uint8_t value, int row, int col)
{
    const char *hex = "0123456789ABCDEF";

    VGA[row * 80 + col] =
        ((uint16_t)0x0F << 8) | hex[(value >> 4) & 0xF];

    VGA[row * 80 + col + 1] =
        ((uint16_t)0x0F << 8) | hex[value & 0xF];
}

void irq_handler(uint64_t *stack)
{
    uint64_t vector = stack[15];

    if (vector == 32) {
        pit_tick();
        pic_eoi(0);
    }
    else if (vector == 33) {
        keyboard_irq();

        print_hex8(
            keyboard_last_scancode(),
            20,
            25
        );

        pic_eoi(1);
    }
}
