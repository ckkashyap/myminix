include $(SOURCE_ROOT)/Rules.inc

INCLUDE=$(SOURCE_ROOT)/include
LINKER_SCRIPT=-Xlinker --script=kernel.ld

CFLAGS= -O0 -static -Wall -nostartfiles -nostdlib       \
	-nodefaultlibs -nostdinc -fno-builtin           \
	-DKERNEL_VERSION="\"$(KERNEL_VERSION)\""        \
	-D_KERNEL_ -I$(INCLUDE)

all: $(OBJECTS)
	ld $(OBJECTS) -Tkernel.ld -o kernel
