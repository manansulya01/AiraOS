BUILD := build
ISO := $(BUILD)/AiraOS.iso
KERNEL := $(BUILD)/airaos.bin

CFLAGS := -m64 \
          -ffreestanding \
          -fno-pie \
          -fno-stack-protector \
          -mno-red-zone \
          -mcmodel=kernel \
          -nostdlib \
          -nostartfiles \
          -nodefaultlibs \
          -Iinclude \
          -Wall \
          -Wextra

LDFLAGS := -m elf_x86_64 -T arch/x86_64/linker.ld

.PHONY: all iso run clean

all: iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: arch/x86_64/boot/entry.asm | $(BUILD)
	nasm -f elf64 $< -o $@

$(BUILD)/gdt.o: arch/x86_64/cpu/gdt.asm | $(BUILD)
	nasm -f elf64 $< -o $@

$(BUILD)/idt.o: arch/x86_64/interrupts/idt.asm | $(BUILD)
	nasm -f elf64 $< -o $@

$(BUILD)/isr.o: arch/x86_64/interrupts/isr.asm | $(BUILD)
	nasm -f elf64 $< -o $@

$(BUILD)/io.o: arch/x86_64/hardware/io.asm | $(BUILD)
	nasm -f elf64 $< -o $@

$(BUILD)/kernel.o: kernel/core/main.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/panic.o: kernel/core/panic.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/interrupts.o: kernel/core/interrupts.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/pic.o: kernel/drivers/pic.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/pit.o: kernel/drivers/pit.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/keyboard.o: kernel/drivers/keyboard.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(KERNEL): $(BUILD)/boot.o \
            $(BUILD)/gdt.o \
            $(BUILD)/idt.o \
            $(BUILD)/isr.o \
            $(BUILD)/io.o \
            $(BUILD)/kernel.o \
            $(BUILD)/panic.o \
            $(BUILD)/interrupts.o \
            $(BUILD)/pic.o \
            $(BUILD)/pit.o \
            $(BUILD)/keyboard.o
	ld $(LDFLAGS) -o $@ $^

iso: $(KERNEL)
	rm -rf $(BUILD)/iso
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(KERNEL) $(BUILD)/iso/boot/airaos.bin
	cp boot/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(BUILD)/iso

run: iso
	qemu-system-x86_64 \
		-cdrom $(ISO) \
		-m 256M

clean:
	rm -rf $(BUILD)
