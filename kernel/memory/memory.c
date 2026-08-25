#include <stdint.h>

#include "memory.h"
#include "terminal.h"

#define MULTIBOOT2_TAG_END       0
#define MULTIBOOT2_TAG_MMAP      6
#define MULTIBOOT2_MEMORY_AVAILABLE 1

#define PAGE_SIZE 4096ULL

struct multiboot_tag {
    uint32_t type;
    uint32_t size;
} __attribute__((packed));

struct multiboot_tag_mmap {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
} __attribute__((packed));

struct multiboot_mmap_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed));

static uint64_t total_memory;
static uint64_t usable_memory;
static uint32_t region_count;

static uint64_t align_down(uint64_t value)
{
    return value & ~(PAGE_SIZE - 1);
}

static uint64_t align_up(uint64_t value)
{
    return (value + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}

void memory_init(uint64_t multiboot_info)
{
    total_memory = 0;
    usable_memory = 0;
    region_count = 0;

    uint8_t *current = (uint8_t *)(uintptr_t)(multiboot_info + 8);

    for (;;) {
        struct multiboot_tag *tag =
            (struct multiboot_tag *)current;

        if (tag->type == MULTIBOOT2_TAG_END)
            break;

        if (tag->type == MULTIBOOT2_TAG_MMAP) {
            struct multiboot_tag_mmap *mmap =
                (struct multiboot_tag_mmap *)current;

            uint8_t *entry_ptr = current + 16;
            uint8_t *end = current + mmap->size;

            while (entry_ptr < end) {
                struct multiboot_mmap_entry *entry =
                    (struct multiboot_mmap_entry *)entry_ptr;

                if (entry->type == MULTIBOOT2_MEMORY_AVAILABLE) {
                    uint64_t start = align_up(entry->addr);
                    uint64_t finish =
                        align_down(entry->addr + entry->len);

                    if (finish > start) {
                        uint64_t length = finish - start;

                        usable_memory += length;

                        if (finish > total_memory)
                            total_memory = finish;
                    }

                    region_count++;
                }

                entry_ptr += mmap->entry_size;
            }
        }

        current += (tag->size + 7) & ~7ULL;
    }
}

uint64_t memory_total_bytes(void)
{
    return total_memory;
}

uint64_t memory_usable_bytes(void)
{
    return usable_memory;
}

uint32_t memory_region_count(void)
{
    return region_count;
}

void memory_print_info(void)
{
    terminal_write("Memory information\n");
    terminal_write("-----------------\n");

    terminal_write("Highest usable address: ");
    terminal_write_hex(total_memory);
    terminal_putchar('\n');

    terminal_write("Usable memory: ");
    terminal_write_hex(usable_memory);
    terminal_write(" bytes\n");

    terminal_write("Usable regions: ");
    terminal_write_hex(region_count);
    terminal_putchar('\n');

    terminal_write("Page size: ");
    terminal_write_hex(PAGE_SIZE);
    terminal_write(" bytes\n");

    terminal_write("\nPhysical page allocator: OFFLINE\n");
}
