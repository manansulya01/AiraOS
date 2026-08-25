#ifndef AIRAOS_PIT_H
#define AIRAOS_PIT_H

#include <stdint.h>

void pit_init(uint32_t frequency);
void pit_tick(void);
uint64_t pit_get_ticks(void);

#endif
