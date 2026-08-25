#ifndef AIRAOS_IO_H
#define AIRAOS_IO_H

#include <stdint.h>

uint8_t io_inb(uint16_t port);
void io_outb(uint16_t port, uint8_t value);

#endif
