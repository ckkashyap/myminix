.globl __bsp_entry

__bsp_entry:
		cli
		mov $boot_stack, %esp
		add $0x1000, %esp

       	pushl %ebx
       	pushl %eax
       	call init
