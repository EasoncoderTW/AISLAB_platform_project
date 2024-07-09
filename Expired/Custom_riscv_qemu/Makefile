.PHONY: all clean

all:
	riscv64-unknown-elf-gcc -nostartfiles -Tvirt.lds boot.s main.c adder_driver.c adder_driver.h -o main -mcmodel=medany
	qemu-system-riscv64 -nographic -machine virt -bios none -kernel main
clean:
	rm main
