BUILD := build
ISO := $(BUILD)/AiraOS.iso
KERNEL := $(BUILD)/airaos.bin

CFLAGS := -m32 \
          -ffreestanding \
          -fno-pie \
          -fno-stack-protector \
          -nostdlib \
          -nostartfiles \
          -nodefaultlibs \
          -Wall \
          -Wextra

LDFLAGS := -m elf_i386 -T arch/x86_64/linker.ld

.PHONY: all clean iso run

all: iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: arch/x86_64/boot.asm | $(BUILD)
	nasm -f elf32 $< -o $@

$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(KERNEL): $(BUILD)/boot.o $(BUILD)/kernel.o
	ld $(LDFLAGS) -o $@ $^

iso: $(KERNEL)
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(KERNEL) $(BUILD)/iso/boot/airaos.bin
	cp boot/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(BUILD)/iso

run: iso
	qemu-system-x86_64 \
		-cdrom $(ISO) \
		-m 256M \
		-serial stdio

clean:
	rm -rf $(BUILD)
