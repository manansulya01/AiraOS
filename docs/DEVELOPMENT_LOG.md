# AiraOS Development Log

Official engineering record for AiraOS.

## Project

- Name: AiraOS
- Architecture: x86_64
- Repository: https://github.com/manansulya01/AiraOS
- Branch: main
- Development: WSL2 / Ubuntu
- Testing: QEMU

---

# v0.1 — Bootable Kernel

**Status: COMPLETE**

AiraOS repository and initial bootable kernel were established.

## Achieved

- Git repository
- GitHub repository
- main branch
- Project structure
- GRUB boot process
- QEMU testing
- Initial kernel
- VGA output

## Commits

- `e740304` — Initial AiraOS project structure
- `5ccd6d8` — AiraOS v0.1: bootable kernel

---

# v0.2 — x86_64 Long Mode

**Status: COMPLETE**

## Objective

Transition AiraOS into a proper x86_64 long-mode kernel.

## Current files

- `arch/x86_64/boot/entry.asm`
- `arch/x86_64/linker.ld`
- `kernel/core/main.c`
- `Makefile`

## Current blocker

The first v0.2 NASM build failed because page-table entries used symbol expressions with the `|` operator.

NASM reported: `error: | operator may only be applied to scalar values`.

The page-table expressions need to be corrected and the kernel must then be rebuilt and tested in QEMU.

## Verification

- Source: IN PROGRESS
- Compilation: FAILED / DEBUGGING
- ISO: PENDING
- QEMU: PENDING
- Commit: PENDING
- GitHub: PENDING

---

# Development Rules

Every milestone follows:

IMPLEMENT → BUILD → TEST → DOCUMENT → COMMIT → PUSH

A milestone is not complete until every stage succeeds.

---

# Roadmap

| Version | Component | Status |
|---|---|---|
| v0.1 | Bootable kernel | COMPLETE |
| v0.2 | x86_64 long mode | COMPLETE |
| v0.3 | GDT / IDT / exceptions | TODO |
| v0.4 | Physical memory manager | TODO |
| v0.5 | Virtual memory | TODO |
| v0.6 | Kernel heap | TODO |
| v0.7 | Interrupts / timers | TODO |
| v0.8 | Hardware drivers | TODO |
| v0.9 | Processes / scheduler | TODO |
| v0.10 | System calls | TODO |
| v0.11 | Filesystem | TODO |
| v0.12 | Userland | TODO |
| v0.13 | Shell | TODO |
| v0.14 | Networking | TODO |
| v0.15 | Graphics / framebuffer | TODO |
| v0.16 | Window system | TODO |
| v0.17 | Desktop | TODO |
| v0.18 | Applications | TODO |
| v0.19 | AI subsystem | TODO |
| v1.0 | AiraOS | TODO |

---

# Engineering Principle

AiraOS is developed incrementally. Every significant architectural decision, failure, fix, and completed milestone should be recorded here.

Untested functionality is not considered complete.

---

# v0.2 Verification — COMPLETE

**Date:** 2026-08-25

AiraOS successfully transitioned from the initial boot environment into x86_64 long mode.

## Verified

- NASM assembly succeeds.
- 64-bit ELF kernel links successfully.
- GRUB ISO generation succeeds.
- QEMU boots the generated ISO.
- x86_64 long mode is active.
- Kernel C code executes successfully.
- VGA text output works.
- AiraOS v0.2 kernel reaches its main function.

## Runtime Output

```text
AiraOS
x86_64 long mode active.
AiraOS v0.2 kernel online.
```

## Result

AiraOS v0.2 is officially verified and complete.

---

# v0.3 — CPU Exception & Interrupt Foundation

**Status: IN PROGRESS**

## Objective

Build the foundational CPU protection and interrupt infrastructure for AiraOS.

## Planned

- Global Descriptor Table (GDT)
- Interrupt Descriptor Table (IDT)
- Interrupt service routines (ISR)
- CPU exception handlers
- Kernel panic diagnostics
- QEMU exception verification

## Development State

- v0.2 long mode: COMPLETE
- GDT: COMPLETE
- IDT: COMPLETE
- ISR: COMPLETE
- Exception handling: COMPLETE
- Kernel panic: COMPLETE
- QEMU verification: COMPLETE
- Documentation: COMPLETE



---

# v0.3 Verification — COMPLETE

**Date:** 2026-08-25

AiraOS successfully implemented and verified the foundational x86_64 CPU
exception and interrupt infrastructure.

## Verified

- GDT initialization
- 64-bit code/data segment setup
- IDT with 256 entries
- Interrupt gate configuration
- ISR assembly entry points
- CPU exception dispatch
- Breakpoint exception (INT3)
- C-level exception handler
- Kernel panic diagnostics
- QEMU boot verification

## Verified Exception

- Exception: Breakpoint
- Vector: 3
- Error Code: 0

## Result

AiraOS successfully transitions from x86_64 long mode into protected
interrupt/exception handling and displays a controlled kernel panic.

**v0.3 CPU Exception & Interrupt Foundation: COMPLETE**

---

# Journal Entry — Preemptive Scheduler Runtime Breakthrough

**Date:** 2026-08-26
**Time:** 2026-08-26 05:07:23 UTC
**Milestone:** v0.9 — Initial Preemptive Scheduler Runtime Verification
**Git Commit:** `df30e5f`
**Commit:** `checkpoint: preemptive scheduler context switching`

## Objective

The objective of this development session was to move AiraOS from merely having scheduler source code to actually performing CPU context switching between independent kernel tasks.

The scheduler needed to:

1. Receive a periodic PIT interrupt.
2. Enter the ISR.
3. Preserve the interrupted CPU state.
4. Enter the C interrupt handler.
5. Invoke the scheduler.
6. Select another task.
7. Obtain the selected task's saved stack pointer.
8. Return that stack pointer to the assembly ISR.
9. Replace the active RSP.
10. Restore the selected task's CPU state.
11. Execute `iretq`.
12. Resume execution in the selected task.

## Work Performed

The scheduler subsystem introduced:

- `include/task.h`
- `include/scheduler.h`
- `kernel/core/scheduler.c`
- `kernel/task/task.c`
- `kernel/task/test_tasks.c`

The kernel was also modified to integrate the scheduler with:

- `kernel/core/main.c`
- `kernel/core/interrupts.c`
- `kernel/core/panic.c`
- `arch/x86_64/interrupts/isr.asm`
- `Makefile`

## Task Model

The initial task system introduced task states including:

- `TASK_UNUSED`
- `TASK_READY`
- `TASK_RUNNING`
- `TASK_BLOCKED`

The system supports a maximum of:

`MAX_TASKS = 16`

Each task receives an independent kernel stack.

Initial task stack size:

`16384 bytes`

## Major Debugging Problem

The most difficult part of the scheduler implementation was not selecting another task.

The difficult part was correctly preserving and restoring the CPU execution context.

Early attempts produced problems including:

- page faults
- invalid stack states
- incorrect return addresses
- corrupted execution state
- QEMU crashes
- incorrect task restoration

The central discovery was that the scheduler cannot simply choose another function.

It must restore a complete CPU execution state that is compatible with the interrupt-return mechanism.

The saved stack pointer therefore became the central object used to transfer execution between tasks.

## Critical Architecture

The final initial architecture became:

```
             PIT IRQ0
                |
                v
            ISR entry
                |
                v
        Save CPU registers
                |
                v
          irq_handler()
                |
                v
        scheduler_tick()
                |
                v
        Select next task
                |
                v
       Return selected RSP
                |
                v
            mov rsp, rax
                |
                v
       Restore CPU registers
                |
                v
              iretq
                |
                v
        Resume selected task
```

## Build Verification

A clean build was performed:

```
make clean
make
```

The complete kernel successfully:

- assembled
- compiled
- linked
- generated the GRUB ISO

The build completed successfully.

NASM still produced non-blocking warnings regarding:

```
implicit DEFAULT ABS is deprecated
```

These warnings do not currently prevent AiraOS from building or booting.

## Git Verification

The scheduler checkpoint was committed as:

```
df30e5f checkpoint: preemptive scheduler context switching
```

The commit was successfully pushed to:

```
origin/main
```

The repository was subsequently verified clean.

## First Runtime Test

The resulting AiraOS ISO was booted in QEMU.

The scheduler successfully produced repeated task execution:

```
TASK A
TASK B
TASK A
TASK B
TASK A
TASK B
...
```

This was a major runtime milestone.

The scheduler was no longer merely compiling.

It was actually controlling CPU execution.

## What This Proved

The runtime test demonstrated that AiraOS can currently:

- receive PIT timer interrupts
- enter the interrupt subsystem
- execute scheduler logic
- select another task
- switch stack contexts
- restore execution state
- return through `iretq`
- resume another kernel task

This represents the first successful runtime demonstration of preemptive task switching in AiraOS.

## New Problem Discovered

The scheduler test exposed a new architectural problem.

Once TASK A and TASK B continuously executed, the existing interactive shell was no longer usable normally.

Commands such as:

```
boot
clear
```

could no longer be entered and processed as before.

## Root Cause

The scheduler test tasks were continuously participating in the new scheduling system while the shell remained outside the proper task model.

The current architecture therefore effectively behaves like:

```
             Scheduler
                 |
          +------+------+
          |             |
       TASK A         TASK B

       Shell remains outside
       the task model
```

This means the scheduler itself is functioning, but the rest of the kernel has not yet been fully integrated with multitasking.

## Important Engineering Interpretation

This problem should not be classified as a scheduler failure.

Instead, the scheduler breakthrough revealed the next architectural requirement.

A multitasking kernel needs its interactive components to participate in the same execution model.

The shell eventually needs to become a scheduler-managed execution context.

## Next Architecture

The intended architecture is:

```
                  Scheduler
                      |
          +-----------+-----------+
          |           |           |
          v           v           v
        Shell       Task A      Task B
          |
          v
       Keyboard
          |
          v
      Block / Wake
```

An idle task will eventually handle periods where no runnable task exists.

## Current Problems

### Problem 1 — Shell Integration

**Status:** OPEN

The shell currently does not coexist correctly with the continuous scheduler test tasks.

### Problem 2 — Task Lifecycle

**Status:** IN PROGRESS

The task system needs robust:

- READY handling
- RUNNING handling
- BLOCKED handling
- task termination
- task creation
- task selection

### Problem 3 — Blocking and Wakeup

**Status:** TODO

The shell should eventually be able to sleep while waiting for keyboard input and wake when input becomes available.

### Problem 4 — Idle Task

**Status:** TODO

A dedicated idle task should eventually run when no other task is ready.

### Problem 5 — Scheduler Stress Testing

**Status:** TODO

The current A/B demonstration proves the basic mechanism works, but it is not yet sufficient to declare the scheduler production-stable.

Further testing is required for:

- repeated context switches
- keyboard interrupts
- shell interaction
- multiple tasks
- task blocking
- task wakeup
- long-running execution
- unexpected task termination

## Development Journal

This was an important moment in AiraOS development.

The scheduler had previously existed primarily as an engineering problem involving stack layouts, interrupt frames, register preservation, and QEMU crashes.

On this day, that theory became observable CPU behavior.

Seeing:

```
TASK A
TASK B
TASK A
TASK B
```

appear continuously in QEMU was the first convincing proof that AiraOS could maintain multiple independent execution contexts and move the CPU between them.

The immediate shell failure was equally valuable.

It showed that adding multitasking changes the architecture of the entire kernel.

The shell can no longer be treated as something running beside the scheduler.

It needs to become part of the scheduler.

This is the point where AiraOS begins moving from a collection of kernel subsystems toward a genuinely multitasking operating-system architecture.

## Milestone Result

**Initial preemptive context switching: RUNTIME VERIFIED**

**Scheduler A/B test: SUCCESS**

**Interactive shell integration: OPEN**

**Scheduler robustness: IN PROGRESS**

## Next Objective

The next engineering objective is:

**Integrate the interactive shell into the scheduler without breaking keyboard input or context switching.**

The target architecture is:

```
             AiraOS Scheduler
                    |
        +-----------+-----------+
        |           |           |
      Shell       Task A      Task B
        |                       |
        +-----------+-----------+
                    |
                 Idle Task
```

The goal is to make multitasking and interactive shell operation coexist safely.

---

# Development Journal Rule — Added 2026-08-26

From this point forward, every significant AiraOS development milestone must record:

- Date
- Exact time when available
- Objective
- Files changed
- Commands used
- Build result
- Runtime result
- Problems encountered
- Root cause
- Fix
- Verification
- Git commit
- Git push
- Current state
- Next objective
- Personal engineering journal entry

The permanent development cycle is:

```
IMPLEMENT
    |
    v
BUILD
    |
    v
TEST
    |
    v
DOCUMENT
    |
    v
COMMIT
    |
    v
PUSH
    |
    v
VERIFY
```

A feature is not considered fully complete merely because it compiles.

It must survive actual execution and be documented in the engineering journal.
