#ifndef AIRAOS_SCHEDULER_H
#define AIRAOS_SCHEDULER_H

#include <stdint.h>

void scheduler_init(void);
uint64_t scheduler_tick(uint64_t current_rsp);
void scheduler_start(void);

#endif
