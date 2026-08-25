#include <stdint.h>

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static void clear_screen(void)
{
    for (int i = 0; i < 80 * 25; i++)
        VGA[i] = ((uint16_t)0x07 << 8) | ' ';
}

static void print_at(const char *text, int row, int col)
{
    while (*text && col < 80) {
        VGA[row * 80 + col] =
            ((uint16_t)0x0F << 8) | (uint8_t)*text;
        text++;
        col++;
    }
}

static void print_hex(uint64_t value, int row, int col)
{
    const char *hex = "0123456789ABCDEF";

    print_at("0x", row, col);
    col += 2;

    for (int i = 15; i >= 0; i--) {
        VGA[row * 80 + col++] =
            ((uint16_t)0x0F << 8) |
            (uint8_t)hex[(value >> (i * 4)) & 0xF];
    }
}

static const char *exception_name(uint64_t vector)
{
    switch (vector) {
        case 0:  return "DIVIDE ERROR";
        case 1:  return "DEBUG";
        case 2:  return "NON-MASKABLE INTERRUPT";
        case 3:  return "BREAKPOINT";
        case 4:  return "OVERFLOW";
        case 5:  return "BOUND RANGE";
        case 6:  return "INVALID OPCODE";
        case 7:  return "DEVICE NOT AVAILABLE";
        case 8:  return "DOUBLE FAULT";
        case 10: return "INVALID TSS";
        case 11: return "SEGMENT NOT PRESENT";
        case 12: return "STACK SEGMENT FAULT";
        case 13: return "GENERAL PROTECTION FAULT";
        case 14: return "PAGE FAULT";
        case 16: return "X87 FLOATING POINT";
        case 17: return "ALIGNMENT CHECK";
        case 18: return "MACHINE CHECK";
        case 19: return "SIMD FLOATING POINT";
        case 20: return "VIRTUALIZATION";
        default: return "UNKNOWN EXCEPTION";
    }
}

void kernel_panic(uint64_t vector, uint64_t error_code)
{
    clear_screen();

    print_at("========================================", 5, 20);
    print_at("       AIRAOS KERNEL PANIC", 7, 20);
    print_at("========================================", 9, 20);

    print_at("Exception:", 12, 20);
    print_at(exception_name(vector), 12, 32);

    print_at("Vector:", 14, 20);
    print_hex(vector, 14, 32);

    print_at("Error Code:", 16, 20);
    print_hex(error_code, 16, 32);

    print_at("CPU HALTED.", 19, 20);

    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}

void exception_handler(uint64_t *stack)
{
    /*
     * Stack layout:
     * [0..14] pushed registers
     * [15]    vector
     * [16]    error code
     *
     * For exceptions without CPU-pushed error codes, the ISR
     * supplies a synthetic zero.
     */
    uint64_t vector = stack[15];
    uint64_t error  = stack[16];

    kernel_panic(vector, error);
}
