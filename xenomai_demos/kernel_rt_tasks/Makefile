XENO=/usr/xenomai
SKIN=native
XENO_CONFIG=$(XENO)/bin/xeno-config
XENO_CFLAGS=$(shell $(XENO_CONFIG) --skin=$(SKIN) --cflags)

EXTRA_CFLAGS+=$(XENO_CFLAGS)

obj-m:=data_acq2.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean

