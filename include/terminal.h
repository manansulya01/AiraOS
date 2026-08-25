#ifndef AIRAOS_TERMINAL_H
#define AIRAOS_TERMINAL_H

#include <stdint.h>

void terminal_init(void);
void terminal_clear(void);
void terminal_putchar(char c);
void terminal_write(const char *text);
void terminal_write_hex(uint64_t value);
void terminal_backspace(void);

#endif
