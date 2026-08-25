#include <stdint.h>
#include "io.h"

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40

static volatile uint64_t ticks = 0;

void pit_init(uint32_t frequency)
{
    uint32_t divisor = 1193182 / frequency;

    io_outb(PIT_COMMAND, 0x36);
    io_outb(PIT_CHANNEL0, divisor & 0xFF);
    io_outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void pit_tick(void)
{
    ticks++;
}

uint64_t pit_get_ticks(void)
{
    return ticks;
}
