#include <stdint.h>
#include "shell.h"
#include "terminal.h"
#include "memory.h"
#include "pit.h"
#include "keyboard.h"

#define SHELL_BUFFER_SIZE 128

static char command_buffer[SHELL_BUFFER_SIZE];
static uint32_t command_length;

static void shell_prompt(void)
{
    terminal_write("aira> ");
}

static int string_equals(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b)
            return 0;

        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

static void shell_execute(void)
{
    command_buffer[command_length] = '\0';

    terminal_putchar('\n');

    if (command_length == 0) {
        shell_prompt();
        return;
    }

    if (string_equals(command_buffer, "help")) {
        terminal_write("AiraOS v0.4 kernel shell\n");
        terminal_write("Commands:\n");
        terminal_write("  help    - show this help\n");
        terminal_write("  clear   - clear the screen\n");
        terminal_write("  echo    - print text\n");
        terminal_write("  info    - kernel information\n");
        terminal_write("  ticks   - timer tick counter\n");
        terminal_write("  uptime  - system uptime\n");
        terminal_write("  reboot  - reboot the machine\n");
    }
    else if (string_equals(command_buffer, "clear")) {
        terminal_clear();
    }
    else if (string_equals(command_buffer, "mem")) {
        memory_print_info();
    }
    else if (string_equals(command_buffer, "info")) {
        terminal_write("AiraOS v0.4\n");
        terminal_write("Architecture: x86_64\n");
        terminal_write("Mode: long mode\n");
        terminal_write("Interrupts: enabled\n");
        terminal_write("PIC: online\n");
        terminal_write("PIT: 100 Hz\n");
        terminal_write("Keyboard: PS/2 IRQ1\n");
    }
    else if (string_equals(command_buffer, "ticks")) {
        terminal_write("Timer ticks: ");
        terminal_write_hex(pit_get_ticks());
        terminal_putchar('\n');
    }
    else if (string_equals(command_buffer, "uptime")) {
        terminal_write("Uptime: ");
        terminal_write_hex(pit_get_ticks());
        terminal_write(" ticks\n");
    }
    else if (string_equals(command_buffer, "reboot")) {
        terminal_write("Reboot requested.\n");
        terminal_write("Reboot is not implemented yet.\n");
    }
    else if (command_buffer[0] == 'e' &&
             command_buffer[1] == 'c' &&
             command_buffer[2] == 'h' &&
             command_buffer[3] == 'o' &&
             command_buffer[4] == ' ') {
        terminal_write(command_buffer + 5);
        terminal_putchar('\n');
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command_buffer);
        terminal_putchar('\n');
        terminal_write("Type 'help' for available commands.\n");
    }

    command_length = 0;
    shell_prompt();
}

void shell_init(void)
{
    command_length = 0;
}

void shell_task(void)
{
    shell_prompt();

    for (;;) {
        char c = keyboard_get_char();

        if (c)
            shell_handle_char(c);

        __asm__ volatile ("pause");
    }
}

void shell_handle_char(char c)
{
    if (c == '\b') {
        if (command_length > 0) {
            command_length--;
            terminal_backspace();
        }

        return;
    }

    if (c == '\n') {
        shell_execute();
        return;
    }

    if (c < 32 || c > 126)
        return;

    if (command_length >= SHELL_BUFFER_SIZE - 1)
        return;

    command_buffer[command_length++] = c;
    terminal_putchar(c);
}
