#include <stdint.h>

#include "framebuffer.h"

#define MULTIBOOT2_TAG_END         0
#define MULTIBOOT2_TAG_FRAMEBUFFER 8

typedef struct {
    uint32_t type;
    uint32_t size;
} multiboot_tag_t;

typedef struct {
    uint32_t type;
    uint32_t size;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint16_t reserved;
} multiboot_tag_framebuffer_t;

static framebuffer_t fb;

void framebuffer_init(uint64_t multiboot_info)
{
    fb.address = 0;
    fb.width = 0;
    fb.height = 0;
    fb.pitch = 0;
    fb.bpp = 0;
    fb.type = 0;
    fb.enabled = 0;

    if (!multiboot_info)
        return;

    uint8_t *current =
        (uint8_t *)(uintptr_t)(multiboot_info + 8);

    for (;;) {
        multiboot_tag_t *tag =
            (multiboot_tag_t *)current;

        if (tag->type == MULTIBOOT2_TAG_END)
            break;

        if (tag->type == MULTIBOOT2_TAG_FRAMEBUFFER) {
            multiboot_tag_framebuffer_t *framebuffer =
                (multiboot_tag_framebuffer_t *)current;

            fb.address = framebuffer->framebuffer_addr;
            fb.pitch = framebuffer->framebuffer_pitch;
            fb.width = framebuffer->framebuffer_width;
            fb.height = framebuffer->framebuffer_height;
            fb.bpp = framebuffer->framebuffer_bpp;
            fb.type = framebuffer->framebuffer_type;

            if (fb.address &&
                fb.width &&
                fb.height &&
                fb.pitch &&
                fb.bpp == 32) {

                fb.enabled = 1;
            }

            break;
        }

        if (tag->size < 8)
            break;

        current += (tag->size + 7) & ~7ULL;
    }
}

const framebuffer_t *framebuffer_get(void)
{
    return &fb;
}

void framebuffer_put_pixel(
    uint32_t x,
    uint32_t y,
    uint32_t color
)
{
    if (!fb.enabled)
        return;

    if (x >= fb.width || y >= fb.height)
        return;

    volatile uint32_t *buffer =
        (volatile uint32_t *)(uintptr_t)fb.address;

    buffer[(y * fb.pitch / 4) + x] = color;
}

void framebuffer_clear(uint32_t color)
{
    if (!fb.enabled)
        return;

    for (uint32_t y = 0; y < fb.height; y++) {
        for (uint32_t x = 0; x < fb.width; x++) {
            framebuffer_put_pixel(x, y, color);
        }
    }
}

void framebuffer_fill_rect(
    uint32_t x,
    uint32_t y,
    uint32_t width,
    uint32_t height,
    uint32_t color
)
{
    if (!fb.enabled)
        return;

    if (x >= fb.width || y >= fb.height)
        return;

    if (x + width > fb.width)
        width = fb.width - x;

    if (y + height > fb.height)
        height = fb.height - y;

    for (uint32_t py = y; py < y + height; py++) {
        for (uint32_t px = x; px < x + width; px++) {
            framebuffer_put_pixel(px, py, color);
        }
    }
}
