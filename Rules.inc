INCLUDE=-I. -I $(SOURCE_ROOT)/include
CC=gcc


C_SOURCES=$(wildcard *.c)
ASM_SOURCES=$(wildcard *.S)
SOURCES=$(C_SOURCES) $(ASM_SOURCES)
C_OBJECTS=$(patsubst %.c,%.o,$(C_SOURCES))
ASM_OBJECTS=$(patsubst %.S,%.o,$(ASM_SOURCES))
OBJECTS=$(C_OBJECTS) $(ASM_OBJECTS)

%o:%S
	$(CC) -c $(INCLUDE) $< -o $@

dummy: all
	

clean:
	exec find . -name "*.o" -exec rm -f {} \;
