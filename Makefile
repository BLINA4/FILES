obj-m := files.o

all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
