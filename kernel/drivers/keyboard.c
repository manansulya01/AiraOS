#include <stdint.h>

#include "io.h"
#include "keyboard.h"

#define KEYBOARD_DATA 0x60
#define KEYBOARD_BUFFER_SIZE 128

static volatile uint8_t last_scancode = 0;

static volatile char input_buffer[KEYBOARD_BUFFER_SIZE];
static volatile uint32_t input_head = 0;
static volatile uint32_t input_tail = 0;

static const char scancode_table[128] = {
    0,
    27,
    '1','2','3','4','5','6','7','8','9','0','-','=',
    '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',
    0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,
    '\\',
    'z','x','c','v','b','n','m',',','.','/',
    0,
    '*',
    0,
    ' ',
};

void keyboard_init(void)
{
    last_scancode = 0;
    input_head = 0;
    input_tail = 0;
}

void keyboard_irq(void)
{
    uint8_t scancode = io_inb(KEYBOARD_DATA);

    last_scancode = scancode;

    char c = keyboard_scancode_to_ascii(scancode);

    if (!c)
        return;

    uint32_t next =
        (input_head + 1) % KEYBOARD_BUFFER_SIZE;

    /*
     * If the queue is full, discard the newest character.
     * This prevents the IRQ handler from overwriting unread input.
     */
    if (next == input_tail)
        return;

    input_buffer[input_head] = c;
    input_head = next;
}

char keyboard_get_char(void)
{
    if (input_tail == input_head)
        return 0;

    char c = input_buffer[input_tail];

    input_tail =
        (input_tail + 1) % KEYBOARD_BUFFER_SIZE;

    return c;
}

uint8_t keyboard_last_scancode(void)
{
    return last_scancode;
}

char keyboard_scancode_to_ascii(uint8_t scancode)
{
    if (scancode & 0x80)
        return 0;

    if (scancode >= 128)
        return 0;

    return scancode_table[scancode];
}
