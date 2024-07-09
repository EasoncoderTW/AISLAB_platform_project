.PHONY: all clean

all:
	riscv64-unknown-elf-gcc -nostartfiles -Tvirt.lds boot.s main.c adder_driver.c adder_driver.h -o main -mcmodel=medany

clean:
	rm main

run: 
	qemu-system-riscv64 \
	-nographic \
	-machine virt_ais \
	-bios none \
	-kernel main \
	-net none \
	-chardev stdio,id=con,mux=on \
	-serial chardev:con \
	-mon chardev=con,mode=readline