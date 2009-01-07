#include <types.h>
#include <multiboot.h>

/*
 * boot_stack is the stack used by the BSP (Boot Strap Processor)
 * See jump.S, boot_stack is set as the stack before jumping
 * to the init function
 */ 
uint32_t boot_stack[4096] __attribute__ ((aligned (4096)));

void init( unsigned int magic, multiboot_info_t *mb_info ){
	int *p,*c;
	memcpy(p,c,100);
}
