#ifndef AIRAOS_TASK_H
#define AIRAOS_TASK_H

#include <stdint.h>

#define TASK_STACK_SIZE 16384
#define MAX_TASKS 16

typedef enum {
    TASK_UNUSED = 0,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_TERMINATED
} task_state_t;

typedef struct task {
    uint64_t id;
    task_state_t state;

    uint64_t rsp;
    uint64_t stack_base;
    uint64_t stack_size;

    uint64_t ticks;
    uint64_t time_slice;

    struct task *next;
} task_t;

void task_init(void);
task_t *task_create(void (*entry)(void));
task_t *task_current(void);

uint64_t task_schedule(uint64_t current_rsp);

#endif
