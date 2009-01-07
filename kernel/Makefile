include $(SOURCE_ROOT)/Rules.inc

INCLUDE=$(SOURCE_ROOT)/include
LINKER_SCRIPT=-Xlinker --script=kernel.ld

SUB_DIRS=lib cpu memory

all: $(OBJECTS) process_sub_dirs
	ld $(OBJECTS) $(LIBRARIES) -Tkernel.ld -o kernel

clean: clean_sub_dirs
	rm -f $(OBJECTS) kernel *.a
