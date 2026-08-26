#include <stdint.h>

#include "task.h"

static task_t tasks[MAX_TASKS];

static uint8_t stacks[MAX_TASKS][TASK_STACK_SIZE]
    __attribute__((aligned(16)));

static task_t *current = 0;
static uint64_t next_id = 1;

static uint64_t *build_initial_stack(
    uint8_t *stack,
    void (*entry)(void)
)
{
    /*
     * irq_common expects:
     *
     *   rax
     *   rbx
     *   rcx
     *   rdx
     *   rsi
     *   rdi
     *   rbp
     *   r8
     *   r9
     *   r10
     *   r11
     *   r12
     *   r13
     *   r14
     *   r15
     *   vector
     *   error
     *
     * followed by the 64-bit IRET frame:
     *
     *   RIP
     *   CS
     *   RFLAGS
     *   RSP
     *   SS
     *
     * In long mode, IRET restores SS:RSP as part of the
     * interrupt return frame.
     */

    uint64_t *top =
        (uint64_t *)(stack + TASK_STACK_SIZE);

    /*
     * Build backwards.
     *
     * Final memory layout from low to high:
     *
     *   registers
     *   vector
     *   error
     *   RIP
     *   CS
     *   RFLAGS
     *   RSP
     *   SS
     *
     * After irq_common pops the registers and skips vector/error,
     * RSP points at RIP.
     *
     * iretq then restores:
     *   RIP, CS, RFLAGS, RSP, SS
     */

    uint64_t *sp = top;

    /* Long-mode IRET frame */
    *--sp = 0x10;              /* SS */
    *--sp = (uint64_t)top;     /* RSP */
    *--sp = 0x202;             /* RFLAGS */
    *--sp = 0x08;              /* CS */
    *--sp = (uint64_t)entry;   /* RIP */

    /* Software-added interrupt metadata */
    *--sp = 0;                 /* error */
    *--sp = 0;                 /* vector */

    /* Saved general-purpose registers */
    *--sp = 0;                 /* r15 */
    *--sp = 0;                 /* r14 */
    *--sp = 0;                 /* r13 */
    *--sp = 0;                 /* r12 */
    *--sp = 0;                 /* r11 */
    *--sp = 0;                 /* r10 */
    *--sp = 0;                 /* r9 */
    *--sp = 0;                 /* r8 */
    *--sp = 0;                 /* rbp */
    *--sp = 0;                 /* rdi */
    *--sp = 0;                 /* rsi */
    *--sp = 0;                 /* rdx */
    *--sp = 0;                 /* rcx */
    *--sp = 0;                 /* rbx */
    *--sp = 0;                 /* rax */

    return sp;
}

void task_init(void)
{
    for (uint64_t i = 0; i < MAX_TASKS; i++) {
        tasks[i].id = 0;
        tasks[i].state = TASK_UNUSED;
        tasks[i].rsp = 0;
        tasks[i].stack_base = 0;
        tasks[i].stack_size = 0;
        tasks[i].ticks = 0;
        tasks[i].time_slice = 0;
        tasks[i].next = 0;
    }

    current = 0;
}

task_t *task_create(void (*entry)(void))
{
    for (uint64_t i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].state != TASK_UNUSED &&
            tasks[i].state != TASK_TERMINATED)
            continue;

        tasks[i].id = next_id++;
        tasks[i].state = TASK_READY;
        tasks[i].stack_base = (uint64_t)stacks[i];
        tasks[i].stack_size = TASK_STACK_SIZE;
        tasks[i].time_slice = 10;
        tasks[i].ticks = 0;

        tasks[i].rsp =
            (uint64_t)build_initial_stack(
                stacks[i],
                entry
            );

        tasks[i].next = 0;

        return &tasks[i];
    }

    return 0;
}

task_t *task_current(void)
{
    return current;
}

uint64_t task_schedule(uint64_t current_rsp)
{
    /*
     * First scheduler activation:
     * the interrupted context is kernel_main64().
     * There is no task context to save yet, so select
     * the first READY task directly.
     */
    if (!current) {
        for (uint64_t i = 0; i < MAX_TASKS; i++) {
            if (tasks[i].state == TASK_READY) {
                current = &tasks[i];
                current->state = TASK_RUNNING;
                current->ticks = 0;
                current->time_slice = 10;

                return current->rsp;
            }
        }

        return current_rsp;
    }

    /*
     * Save the interrupted task's exact interrupt-stack pointer.
     */
    current->rsp = current_rsp;
    current->ticks++;

    /*
     * Find the next READY task.
     */
    uint64_t current_index =
        (uint64_t)(current - tasks);

    for (uint64_t offset = 1;
         offset <= MAX_TASKS;
         offset++) {

        uint64_t index =
            (current_index + offset) % MAX_TASKS;

        if (tasks[index].state == TASK_READY) {
            current->state = TASK_READY;
            current = &tasks[index];
            current->state = TASK_RUNNING;

            return current->rsp;
        }
    }

    return current_rsp;
}
