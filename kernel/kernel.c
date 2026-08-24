#include <stdint.h>

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;

static void clear_screen(void)
{
    for (int i = 0; i < 80 * 25; i++)
        VGA[i] = (uint16_t)' ' | ((uint16_t)0x07 << 8);
}

static void print(const char *text, int row)
{
    int col = 0;

    while (text[col] != '\0') {
        VGA[row * 80 + col] =
            (uint16_t)text[col] | ((uint16_t)0x0F << 8);
        col++;
    }
}

void kernel_main(uint32_t magic, uint32_t multiboot_info)
{
    (void)magic;
    (void)multiboot_info;

    clear_screen();

    print("AiraOS", 8);
    print("x86_64 kernel booted successfully.", 10);
    print("AiraOS v0.1 - QEMU development build", 12);

    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}
