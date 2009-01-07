#include <types.h>

#include <cpu/gdt.h>



#define GDTENTRY( base, length, flags, type, access )	\
		{ (uint16_t)(length & 0xFFFF),					\
		  (uint16_t)(base & 0xFFFF),					\
		  (uint8_t)((base>>16) & 0xFF),					\
		  (uint8_t)(flags | access),					\
		  (uint8_t)(type | ((length >> 16) & 0xF)),		\
		  (uint8_t)((base >> 24) & 0xF)					\
		}


/** The temporary GDT table for AP processors */
segment_descriptor tempGDT[3] __attribute__ ((aligned (4096))) 
	= { 
		GDTENTRY(0,0,0,0,0),
		GDTENTRY(0, 0xFFFFFF,  
				G_CODE |  G_PRESENT | G_READABLE | G_APPLICATION,  
				G_USE32 | G_AVAIL | G_GRANULAR ,  G_DPL0) ,
		GDTENTRY(0, 0xFFFFFF,  
				G_DATA |  G_PRESENT | G_WRITABLE | G_APPLICATION,  
				G_USE32 | G_AVAIL | G_GRANULAR ,  G_DPL0),
	};
	



// Set up the tempGDT NULL descriptor accurately for the AP's. 
void init_gdt()
{
	*(uint16_t*)( tempGDT ) 			= (uint16_t)0xFFFFU;
	*(uint32_t*)( (void*)tempGDT + 2 )  = (uint32_t)tempGDT;
	*(uint16_t*)( (void*)tempGDT + 6 )	= 0;
}


void gdt_update_segments(unsigned int new_code, unsigned int new_data)
{
  asm (
	     "pushl %0\n"
	     "pushl $1f\n"
         "lret \n"
		"1: mov %1, %%eax\n"
		 "mov %%ax, %%ds\n"
		 "mov %%ax, %%es\n"
		 "mov %%ax, %%fs\n"
		 "mov %%ax, %%gs\n"
		 "mov %%ax, %%ss\n"
	   : 
	   : "g" (new_code), "g" (new_data)
	   : "eax");
}




int gdt_new_segment(
		segment_descriptor *gdt,
		uint32_t base, 
		uint32_t length, 
		uint8_t flags, 
		uint8_t type, 
		uint32_t access
		)
{
	int pos;
	for ( pos = 1; pos < MAX_GDT; pos++ )
	{
		if ( gdt[pos].type == 0 )
		{
			gdt[pos].seg_length0_15 = (uint16_t)(length & 0xFFFF);
			gdt[pos].base_addr0_15 = (uint16_t)(base & 0xFFFF);
			gdt[pos].base_addr16_23 = (uint8_t)((base>>16) & 0xFF);
			gdt[pos].flags = flags | access;
			gdt[pos].type = type | ((length >> 16) & 0xf);
			gdt[pos].base_addr24_31 = (uint8_t)((base>>24)&0xF);
			return pos * 8;
		}
	}

	return -1;
}



