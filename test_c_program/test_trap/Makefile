.PHONY: all clean

all:
	riscv64-unknown-elf-gcc -nostartfiles -Tvirt.lds boot.s main.c -o main -mcmodel=medany
	qemu-system-riscv64 -nographic -machine virt -bios none -kernel main

clean:
	rm main
