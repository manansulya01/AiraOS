#include <stdint.h>

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static void clear_screen(void)
{
    for (int i = 0; i < 80 * 25; i++)
        VGA[i] = ((uint16_t)0x07 << 8) | ' ';
}

static void print(const char *text, int row)
{
    for (int col = 0; text[col] != '\0'; col++)
        VGA[row * 80 + col] =
            ((uint16_t)0x0F << 8) | (uint8_t)text[col];
}

void kernel_main64(uint64_t magic, uint64_t multiboot_info)
{
    (void)magic;
    (void)multiboot_info;

    clear_screen();

    print("AiraOS", 8);
    print("x86_64 long mode active.", 10);
    print("AiraOS v0.2 kernel online.", 12);

    for (;;)
        __asm__ volatile ("cli; hlt");
}
