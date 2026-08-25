#include <stdint.h>
#include "terminal.h"

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static uint8_t row;
static uint8_t column;
static uint8_t attribute = 0x0F;

static uint16_t vga_entry(char c)
{
    return ((uint16_t)attribute << 8) | (uint8_t)c;
}

void terminal_clear(void)
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        VGA[i] = vga_entry(' ');

    row = 0;
    column = 0;
}

void terminal_init(void)
{
    terminal_clear();
}

static void terminal_scroll(void)
{
    for (int r = 1; r < VGA_HEIGHT; r++) {
        for (int c = 0; c < VGA_WIDTH; c++)
            VGA[(r - 1) * VGA_WIDTH + c] =
                VGA[r * VGA_WIDTH + c];
    }

    for (int c = 0; c < VGA_WIDTH; c++)
        VGA[(VGA_HEIGHT - 1) * VGA_WIDTH + c] = vga_entry(' ');

    row = VGA_HEIGHT - 1;
    column = 0;
}

void terminal_putchar(char c)
{
    if (c == '\n') {
        column = 0;
        row++;

        if (row >= VGA_HEIGHT)
            terminal_scroll();

        return;
    }

    if (c == '\r') {
        column = 0;
        return;
    }

    if (c == '\b') {
        terminal_backspace();
        return;
    }

    VGA[row * VGA_WIDTH + column] = vga_entry(c);
    column++;

    if (column >= VGA_WIDTH) {
        column = 0;
        row++;

        if (row >= VGA_HEIGHT)
            terminal_scroll();
    }
}

void terminal_write(const char *text)
{
    while (*text)
        terminal_putchar(*text++);
}

void terminal_write_hex(uint64_t value)
{
    const char *hex = "0123456789ABCDEF";

    terminal_write("0x");

    for (int i = 15; i >= 0; i--)
        terminal_putchar(hex[(value >> (i * 4)) & 0xF]);
}

void terminal_backspace(void)
{
    if (column > 0) {
        column--;
        VGA[row * VGA_WIDTH + column] = vga_entry(' ');
    }
}
