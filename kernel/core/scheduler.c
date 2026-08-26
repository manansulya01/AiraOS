#include <stdint.h>

#include "task.h"
#include "scheduler.h"
#include "terminal.h"

#define TASK_TIME_SLICE 10

static uint64_t scheduler_ticks = 0;

static void scheduler_task_bootstrap(void)
{
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

void scheduler_init(void)
{
    scheduler_ticks = 0;
    task_init();
}

uint64_t scheduler_tick(uint64_t current_rsp)
{
    scheduler_ticks++;

    task_t *current = task_current();

    /*
     * First scheduler activation:
     * no task is running yet, so select the first READY task.
     */
    if (!current)
        return task_schedule(current_rsp);

    if (current->time_slice > 0)
        current->time_slice--;

    if (current->time_slice != 0)
        return current_rsp;

    current->time_slice = TASK_TIME_SLICE;

    return task_schedule(current_rsp);
}

void scheduler_start(void)
{
    /*
     * Scheduler is activated by the first PIT interrupt.
     */
    (void)scheduler_task_bootstrap;
}
