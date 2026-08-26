#include <stdint.h>

#include "terminal.h"
#include "pit.h"
#include "task.h"

static void task_a(void)
{
    uint64_t last = 0;

    for (;;) {
        uint64_t now = pit_get_ticks();

        if (now - last >= 100) {
            last = now;
            terminal_write("[TASK A] running\n");
        }

        __asm__ volatile ("pause");
    }
}

static void task_b(void)
{
    uint64_t last = 0;

    for (;;) {
        uint64_t now = pit_get_ticks();

        if (now - last >= 100) {
            last = now;
            terminal_write("[TASK B] running\n");
        }

        __asm__ volatile ("pause");
    }
}

void task_tests_init(void)
{
    task_create(task_a);
    task_create(task_b);
}
