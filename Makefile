GCC = x86_64-elf-gcc
G++ = x86_64-elf-g++
ASM = nasm
RUST = rustc
LD = x86_64-elf-ld

GXX_FLAGS = -O3 -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -nostdlib -Wall -Wextra -c
LD_FLAGS = -m elf_x86_64 -z max-page-size=0x1000 -T kernel/link.ld -nostdlib
ASM_FLAGS = -f elf64

build:
	mkdir out/

	$(ASM) $(ASM_FLAGS) kernel/boot/boot.asm -o out/boot.o
	$(ASM) $(ASM_FLAGS) kernel/main/theweirdstuff/interrupts.asm -o out/interrupts.o

	$(G++) $(GXX_FLAGS) kernel/main/kernel.cpp -o out/kernel.o
	$(G++) $(GXX_FLAGS) kernel/main/gfx/con.c -o out/con.o
	$(G++) $(GXX_FLAGS) kernel/main/gfx/vga.c -o out/vga.o
	$(G++) $(GXX_FLAGS) kernel/main/theweirdstuff/idt.cpp -o out/idt.o
	$(G++) $(GXX_FLAGS) kernel/main/theweirdstuff/pit.c -o out/pit.o
	$(G++) $(GXX_FLAGS) kernel/main/theweirdstuff/kbhandler.c -o out/kbhandler.o
	$(G++) $(GXX_FLAGS) kernel/main/memory/mem.c -o out/mem.o
	$(G++) $(GXX_FLAGS) kernel/main/memory/malloc.c -o out/malloc.o
	$(G++) $(GXX_FLAGS) kernel/main/input/read.c -o out/read.o

	mkdir -p iso/boot/grub

	# link gng ;(
	$(LD) $(LD_FLAGS) out/* -o iso/boot/BMOS.bin

	grub-file --is-x86-multiboot2 iso/boot/BMOS.bin
	cp kernel/boot/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o BlackMesaOS.iso iso/

	@echo "Build Successful!"

disk:
	dd if=/dev/zero of=disk.img bs=1M count=64

clean:
	rm -rf out iso BlackMesaOS.iso
	
cleandisk:
	rm -f disk.img
