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
	segment_descriptor *the_gdt;

	/*
	 * Loop through the GDT to find an empty slot
	 */
	for ( pos = 1; pos < MAX_GDT; pos++ )
	{
		the_gdt = &gdt[pos];
		if ( the_gdt->type == 0 )
		{
			/*
			 * When an empty slot is found
			 * fill it with the details provided
			 */
			the_gdt->seg_length0_15 = (uint16_t)(length & 0xFFFF);
			the_gdt->base_addr0_15 = (uint16_t)(base & 0xFFFF);
			the_gdt->base_addr16_23 = (uint8_t)((base>>16) & 0xFF);
			the_gdt->flags = flags | access;
			the_gdt->type = type | ((length >> 16) & 0xf);
			the_gdt->base_addr24_31 = (uint8_t)((base>>24)&0xF);
			return pos * 8;
		}
	}

	return -1;
}



