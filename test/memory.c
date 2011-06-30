#define	MM_PAGE_SIZE	(64)		/* bytes per page */
#define MM_PAGE_MASK	(0x3F)		/* size mask */
#define MM_PAGE_SHIFT	(6)			/* six bit shift */
#define MM_TABLE_SIZE	(1024)		/* lets start with 512 entres */

typedef struct tag_mm_entry
{
	unsigned int	size;			/* size in pages --- NOTE: these leaves the bottom six bits free for flags */
	void*			previous;
	void*			next;
} MEMORY_ENTRY;

typedef struct
{
	unsigned int	memory_size;
	unsigned int	memory_free;
	void*			memory_pointer;
	unsigned int	allocated;
	unsigned int	free_space;
	unsigned int	unused;
	MEMORY_ENTRY	entry[MM_TABLE_SIZE];

} MEMORY_MANGER;

static	MEMORY_MANGER	memory_manager;

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  InitialiseMemory
 *  Description:  This function will initialise the memory sub-system.
 *-------------------------------------------------------------------------------------*/
void InitialiseMemory ( void* memory_area, unsigned int size )
{
	memset(&memory_manager,0,sizeof(MEMORY_MANGER));

	memory_manager.free_space = (MEMORY_ENTRY*) memory_area;

	memory_manager.free_space->size = (size - sizeof(MEMORY_ENTRY)) & ~MM_PAGE_MASK;
	memory_manager.free_space->free = NULL;
	memory_manager.free_space->prev = NULL;
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  AllocateMemory
 *  Description:  This function will allocate memory.
 *  			  It will add the allocation to the allocation table. It will also 
 *  			  return the memory address, but this is not the start of the allocation
 *  			  as this has 1 int (4 bytes) that holds the original size as this
 *  			  will aid in reallocing (we do this a lot). 
 *-------------------------------------------------------------------------------------*/
void*	AllocateMemory (unsigned int size)
{
	unsigned int	error = UINT_MAX;
	unsigned int	found = UINT_MAX;
	unsigned int	best_fit = UINT_MAX;
	MEMORY_ENTRY*	free_space = memory_manager.free_space;

	required_size = (size + MM_PAGE_MASK) & ~MM_PAGE_MASK;

	while (free_space != NULL)
	{
		if (free_space->size == required_size)
		{

		}

		free_space = free_space->next;
	}
}


