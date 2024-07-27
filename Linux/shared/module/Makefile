ARCH := riscv
CROSS_COMPILE := riscv64-unknown-linux-gnu-
obj-m := adder.o
PWD := $(shell pwd)
KDIR := /workspace/linux
all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) SUBDIR=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) SUBDIR=$(PWD) clean

	                                                        