all:
	make SOURCE_ROOT=$$PWD -C kernel
clean:
	make SOURCE_ROOT=$$PWD -C kernel clean
