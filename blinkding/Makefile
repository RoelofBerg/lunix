#Files to be compiled (.o outputs)
MODULES := lunix.o

#guest architecture
ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-

#Special syntax for Linux kernel build
obj-m := $(MODULES)

#Path of the udoo-neo kernel
KDIR := ~/udooneo-dev/linux_kernel/

#A macro for convenience (less typing below)
MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

#Make targets for creating the kernel modules
modules:
	$(MAKEARCH) -C $(KDIR) M=${shell pwd} modules

#Make target for cleaning up temporary files
clean:
	$(MAKEARCH) -C $(KDIR) M=${shell pwd} clean
