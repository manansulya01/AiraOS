#ifndef AIRAOS_FRAMEBUFFER_H
#define AIRAOS_FRAMEBUFFER_H

#include <stdint.h>

typedef struct {
    uint64_t address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t  bpp;
    uint8_t  type;
    uint8_t  enabled;
} framebuffer_t;

void framebuffer_init(uint64_t multiboot_info);
const framebuffer_t *framebuffer_get(void);

void framebuffer_put_pixel(
    uint32_t x,
    uint32_t y,
    uint32_t color
);

void framebuffer_clear(uint32_t color);

void framebuffer_fill_rect(
    uint32_t x,
    uint32_t y,
    uint32_t width,
    uint32_t height,
    uint32_t color
);

#endif
