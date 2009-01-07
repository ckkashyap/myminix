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

#if 0
	uint32_t count = mboot->mmap_length;
	assert( (count % sizeof(memory_map_t)) == 0 );

	count = count / sizeof(memory_map_t);
	assert( count > 0 );


	map = (memory_map_t*)mboot->mmap_addr;

	parse_map( map, count, callback_add );

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




