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

**Status: IN PROGRESS**

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
| v0.2 | x86_64 long mode | IN PROGRESS |
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
