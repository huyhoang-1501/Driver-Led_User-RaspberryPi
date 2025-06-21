obj-m += driver_rpi4.o
        
KERNEL_DIR ?=/home/huyhoang/kernel_custom/linux
all:
	make -C $(KERNEL_DIR) \
		ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-\
		M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) \
		ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-\
		M=$(PWD) clean
