
obj-m += mb.o

# EXTRA_CFLAGS += XXX

all:
	make -C /lib/modules/`uname -r`/build SUBDIRS=$(PWD) modules

clean:
	make -C /lib/modules/`nname -r`/build SUBDIRS=$(PWD) clean

