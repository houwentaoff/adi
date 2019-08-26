ifneq ($(KERNELRELEASE),)
ad9009-objs := adrv9009.o adrv9009_conv.o \
		talise/talise_agc.o \
		talise/talise_arm.o \
		talise/talise.o \
		talise/talise_cals.o \
		talise/talise_error.o \
		talise/talise_gpio.o \
		talise/talise_hal.o \
		talise/talise_jesd204.o \
		talise/talise_radioctrl.o \
		talise/talise_rx.o \
		talise/talise_tx.o \
		talise/talise_user.o \
		talise/linux_hal.o
obj-m	:= ad9009.o 

else

KERNELDIR ?= /home/work/worke/github/buildroot-2018.02.2/output/build/linux-4.15 #/usr/src/linux-headers-`uname -r`/
PWD       := $(shell pwd)

default:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules 
endif

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers


