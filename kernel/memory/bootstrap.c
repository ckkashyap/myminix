#include <multiboot.h>
#include <memory/bootstrap.h>
#include <types.h>



extern void *_KERNEL_START;
extern void *_KERNEL_END;
static void *KERNEL_START = (void*)&_KERNEL_START;
static void *KERNEL_END = (void*)&_KERNEL_END;


// Note: these ranges are inclusive. So, start = 0, end = 9 => 10 bytes for use.
static void* largest_start = NULL;
static void* largest_end = NULL;
static void* mem_start = NULL;
static void* mem_end = NULL;

typedef void (*callback_t)(void*,void*);


static void add_range( void *start, void *end )
{
	if ( (end - start) > (largest_end - largest_start) )
	{
		largest_start = start;
		largest_end = end;
	}

	if ( start < mem_start ) mem_start = start;
	if ( end > mem_end ) mem_end = end;
}

static void remove_range( void *start, void *end )
{
	if ( (end >= largest_start) && (start <= largest_end) )
	{
		uintptr_t left_diff = 0;
		uintptr_t right_diff = 0;

		if ( start > largest_start ) 
			left_diff = (uintptr_t)(start - largest_start);

		if ( end < largest_end ) 
			right_diff = (uintptr_t)(largest_end - end);

		if ( left_diff < right_diff )
			largest_start = end + 1;
		else{
			kprintf("DINGO\n");
			largest_end = start - 1;
		}
	}

	if ( start < mem_start ) mem_start = start;
	if ( end > mem_end ) mem_end = end;
}

static void parse_map( memory_map_t* map, uint32_t count, callback_t callback)
{
	uint32_t i;
	uint64_t base_addr,base_length;

	for ( i = 0; i < count; i++ )
	{
		base_addr = map[i].base_addr_high;
		base_addr = (base_addr << 32) + map[i].base_addr_low;

		base_length = map[i].length_high;
		base_length = (base_length << 32) + map[i].length_low;
		
		/*
		   As per the multiboot spec, if the type is not 1
		   then it is not usable.
		 */
		if ( map[i].type != 1 ) continue;

		callback( (void*)base_addr, (void*)(base_addr + base_length - 1) );
	}
}

static void parse_modules( module_t* modules, uint32_t count, callback_t callback )
{
	uint32_t i;

	callback( (void*)modules, (void*)(modules + count * sizeof(module_t) - 1) );

	for ( i = 0; i < count; i++ )
	{
		callback( (void*)(modules[i].string), 
				(void*)(modules[i].string + strlen((const char*)modules[i].string) + 1) );
		callback( (void*)(modules[i].mod_start), (void*)(modules[i].mod_end) );
	}
}

/** A few stages here:
 *
 *     1. Scan all of memory for the biggest contiguous available
 *        space.
 *     2. Allocate this space as a buffer for the physmem manager.
 *     3. Add available and remove unavailable ranges from the physmem 
 *        manager.
 *
 */
void bootstrap_memory( multiboot_info_t *mboot )
{
	memory_map_t *map;
	size_t map_size;
	size_t pages;
	uint32_t count;
		
	if (! (mboot->flags & 0x8) ){
		kprintf("No modules!!! - bad\n");
		while(1);
	}
		
	count = mboot->mmap_length;
	count = count / sizeof(memory_map_t);


	map = (memory_map_t*)mboot->mmap_addr;

	/*
	   parse_map goes through various blocks of memory
	   and sets largest_start and largest_end with the
	   largest available contiguous block
	*/
	parse_map( map, count, add_range );

	/*
	   Some memory blocks are not to be used - 
	   - The location where the modules are loaded
	   - The location where the multiboot info is stored
	   - The VGA area
	   - And ofcourse, the kernel itself!

	*/
	parse_modules( (module_t*)(mboot->mods_addr), mboot->mods_count, remove_range );
	remove_range( (void*)mboot, (void*)(mboot + sizeof(multiboot_info_t) - 1));
	remove_range( (void*)0, (void*)0xFFF );	// Never return NULL
	remove_range( (void*)0xB8000, (void*)0xFFFFF ); // No VGA etc
	remove_range( KERNEL_START, KERNEL_END ); // the kernel itself




#if 0
	parse_modules( (module_t*)(mboot->mods_addr), mboot->mods_count,
						callback_rem );


	callback_rem( (void*)mboot, 
							(void*)(mboot + sizeof(multiboot_info_t) - 1));

	callback_rem( (void*)0, (void*)0xFFF );	// Never return NULL
	callback_rem( (void*)0xB8000, (void*)0xFFFFF ); // No VGA etc
	callback_rem( KERNEL_START, KERNEL_END );


	dmesg( "Memory range of (%i) required for management [%p:%p]\n",
					(int)(mem_end - mem_start + 1),
					mem_start, mem_end );
	dmesg( "Found working area of (%i) bytes [%p:%p]\n",
					(int)(largest_end - largest_start + 1),
					largest_start, largest_end );


	pages = (size_t)( (mem_end - mem_start + 1) / PAGE_SIZE );
	map_size = (pages / BLOCK_BITS) * BLOCK_SIZE;
	if ( (pages % BLOCK_BITS) != 0 ) map_size += BLOCK_SIZE;

	dmesg( "Required management size (%i) bytes for (%i) pages at [%p,%p]\n",
					map_size, 
					pages,
					largest_start,
					largest_start + map_size -1 );

	assert( map_size <= (largest_end - largest_start + 1) );

	init_physmem( mem_start, largest_start, pages );

	parse_map( map, count, callback_physmem_add );

	parse_modules( (module_t*)(mboot->mods_addr), mboot->mods_count,
						callback_physmem_rem );

	callback_physmem_rem( (void*)mboot, 
							(void*)(mboot + sizeof(multiboot_info_t) - 1));

	callback_physmem_rem( (void*)0, (void*)0xFFF );	// Never return NULL
	callback_physmem_rem( (void*)0xB8000, (void*)0xFFFFF ); // No VGA etc
	callback_physmem_rem( KERNEL_START, KERNEL_END );
	callback_physmem_rem( largest_start, largest_start + map_size - 1 );


	show_memory_map();
#endif
}




