#ifndef AIRAOS_MEMORY_H
#define AIRAOS_MEMORY_H

#include <stdint.h>

void memory_init(uint64_t multiboot_info);
void memory_print_info(void);

uint64_t memory_total_bytes(void);
uint64_t memory_usable_bytes(void);
uint32_t memory_region_count(void);

#endif
