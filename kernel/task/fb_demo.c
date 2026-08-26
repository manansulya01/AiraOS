#include <stdint.h>

#include "framebuffer.h"
#include "pit.h"

void framebuffer_demo_task(void)
{
    const framebuffer_t *fb = framebuffer_get();

    if (!fb->enabled)
        return;

    const uint32_t box_w = 120;
    const uint32_t box_h = 80;

    if (fb->width < box_w || fb->height < box_h)
        return;

    const uint32_t max_x = fb->width - box_w;
    const uint32_t max_y = fb->height - box_h;

    for (;;) {
        uint32_t t = (uint32_t)(pit_get_ticks() / 2);

        uint32_t x;

        if (max_x == 0) {
            x = 0;
        } else {
            uint32_t period = max_x * 2;

            x = t % period;

            if (x >= max_x)
                x = period - x;
        }

        uint32_t y = max_y / 2;

        framebuffer_clear(0x00101820);

        framebuffer_fill_rect(
            x,
            y,
            box_w,
            box_h,
            0x004080FF
        );

        __asm__ volatile ("pause");
    }
}
