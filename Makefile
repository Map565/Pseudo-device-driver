obj-m = test.o
KVERSION = $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
test:
	#We put a — in front of the rmmod command to tell make to ignore
	# an error in case the module isn’t loaded.
	-sudo rmmod test
	# Clear the kernel log without echo
	sudo dmesg -C
	# Insert the module
	sudo insmod test.ko
	# Display the kernel log
	dmesg
