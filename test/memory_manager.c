#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <limits.h>
#include <malloc.h>

#include <semaphore.h>

#if MEMORY_TEST > 2
#include <execinfo.h>
#endif 

#if MEMORY_TEST == 4
#include <assert.h>
#endif

#define MAX_LEVELS		(10)
#define	MM_PAGE_SIZE	(64)				/* must always be a power of two */
#define MM_PAGE_BITMAP	(MM_PAGE_SIZE - 1)
#define MM_PAGE_MASK	(~(MM_PAGE_BITMAP))

#define MM_MEMORY_HEADER(b)	((b - sizeof(MEMORY_HEADER)))
#define MM_POINTER_CHECK(b)	(((((unsigned long)MM_MEMORY_HEADER(b)) == (((unsigned long)b) & MM_PAGE_MASK)) && (b != NULL)))


typedef struct tag_mem_header
{
	unsigned int			size;			/* this size is page aligned  and the bottom bits are used as flags */
	struct tag_mem_header*	prev_entry;		/* pointer to the previous memory allocation */
	struct tag_mem_header*	next_free;		/* This will be null when the item is allocated, else point to the next free item */
#if MEMORY_TEST > 0
	void*					alloc_by_line;	/* the following are used to handle memory leak tracing */
#if MEMORY_TEST > 1
	void*					freed_by_line;
#endif
#endif	
	
} MEMORY_HEADER;

typedef struct
{
	unsigned int			size;
	void*					memory;
	void*					memory_end;
	sem_t					mutex;
	MEMORY_HEADER			free_space;

#if MEMORY_TEST == 5
	unsigned short			num_allocations;
	unsigned short			num_frees;
	unsigned short			average_wasted;
	unsigned short			average_size;
#endif
} MEMORY_MANGER;

#if MEMORY_TEST == 4
static	unsigned int	duplicate_free = 0;
static	unsigned int	bad_address = 0;
#endif

static	MEMORY_MANGER	manager;

#if MEMORY_TEST > 2

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  DumpSymbol
 *  Description:  This function will dump a specific symbol
 *-------------------------------------------------------------------------------------*/
void DumpSymbol ( void* symbol, char* name )
{
	char**	strings;

	strings = backtrace_symbols(&symbol,1);

	printf("MEMORY: %s %s\n",name, strings[0]);
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  DumpBacktrace
 *  Description:  This function will dump the backtrace to the stdio.
 *-------------------------------------------------------------------------------------*/
void DumpBacktrace (unsigned int levels, char* reason)
{
	int 	size;
	int		count;
	void*	buffer[MAX_LEVELS+2];
	char**	strings;

	if (levels > MAX_LEVELS)
		levels = MAX_LEVELS;

	size = backtrace(buffer,levels+2);
	strings = backtrace_symbols(buffer,size);

	if (strings == NULL)
	{
		printf("failed\n");
	}
	else
	{
		printf("MEMORY: Error reason - %s\nMEMORY: backtrace:\n",reason);

		for (count = 2;count < size; count++)
		{
			printf("MEMORY:     %s\n",strings[count]);
		}
	}

	free(strings);
}
#endif

#if MEMORY_TEST == 5

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  RollingAverage
 *  Description:  This function calculates the rolling average.
 *-------------------------------------------------------------------------------------*/
unsigned int	RollingAverage(unsigned int average,unsigned int data, unsigned int num_points)
{
	return (average + (((int)data - (int)average) / (int) num_points));
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  DumpStatistics
 *  Description:  This function will dump the memory statistics.
 *-------------------------------------------------------------------------------------*/
void DumpStatistics ( void )
{
	printf(	"MEMORY: =========================================\n"
			"MEMORY:          Memory Usage Stats\n"
			"MEMORY: num_allocations: %d\n"
			"MEMORY:       num frees: %d\n"
			"MEMORY:  average wasted: %d\n"
			"MEMORY:    average size: %d\n"
			"MEMORY: =========================================\n"
			,manager.num_allocations,manager.num_frees,manager.average_wasted,manager.average_size);
}
#endif

/*----- FUNCTION -----------------------------------------------------------------*
 * Name : InitialiseMemory
 * Desc : This function will initialise the memory system.
 *--------------------------------------------------------------------------------*/
void	InitialiseMemory(void* memory_start, unsigned int memory_size)
{
	memset(&manager,0,sizeof(MEMORY_MANGER));

	printf("size requestd: %d\n",memory_size);
	printf("size of the header: %d\n",sizeof(MEMORY_HEADER));

	/* init the memory manager */
	manager.size 		= memory_size;
	manager.memory 		= memory_start;
	manager.memory_end	= (void*)(((unsigned long)(memory_start + memory_size)) & MM_PAGE_MASK);
	manager.free_space.next_free	= (void*)(((unsigned long)memory_start + MM_PAGE_BITMAP) & MM_PAGE_MASK);

	/* init the free space item */
	manager.free_space.next_free->size = (memory_size - sizeof(MEMORY_HEADER)) & MM_PAGE_MASK;
	manager.free_space.next_free->next_free = NULL;

	sem_init(&manager.mutex,0,1);
}

/*----- FUNCTION -----------------------------------------------------------------*
 * Name : AllocateMemory
 * Desc : This function will allocate a lump of memory, it will use a best fit
 *        first algorithm.
 *--------------------------------------------------------------------------------*/
void*	AllocateMemory(unsigned int size)
{
	void* 			result = NULL;
	unsigned int	required_size = (size + sizeof(MEMORY_HEADER) + MM_PAGE_BITMAP) & MM_PAGE_MASK;
	unsigned int	error_size = UINT_MAX;
	MEMORY_HEADER	*new_entry;
	MEMORY_HEADER	*prev_fit;
	MEMORY_HEADER	*best_fit = NULL;
	MEMORY_HEADER	*following_entry;
	MEMORY_HEADER	*free_space = manager.free_space.next_free;
	MEMORY_HEADER	*prev_free_space = &manager.free_space;

	sem_wait(&manager.mutex);

	while (free_space != NULL)
	{
#if MEMORY_TEST == 5
		manager.num_allocations++;
		manager.average_size = RollingAverage(manager.average_size,required_size,manager.num_allocations);
		manager.average_wasted = RollingAverage(manager.average_wasted,required_size-size,manager.num_allocations);
#endif		
		if (free_space->size == required_size)
		{
			prev_free_space->next_free = free_space->next_free;
			free_space->next_free = NULL;

			free_space->size |= 0x01;	/* Set the allocated bit */

			result = (void*) ((void*)free_space) + sizeof(MEMORY_HEADER);
			break;
		}
		else if (free_space->size > required_size && (free_space->size - required_size) < error_size)
		{
			best_fit = free_space;
			prev_fit = prev_free_space;
			error_size = free_space->size - required_size;
		}

		prev_free_space = free_space;
		free_space = free_space->next_free;
	}

	if (result == NULL && best_fit != NULL)
	{
		/* ok, we have a winner - we need to split it */
		new_entry = ((void*)best_fit) + required_size;
		new_entry->size = best_fit->size - required_size;
		new_entry->next_free = best_fit->next_free;
		new_entry->prev_entry = best_fit;

		/* adjust the free space chain */
		prev_fit->next_free = new_entry;

		/* now adjust the old element */
		best_fit->size = required_size;
		best_fit->next_free = NULL;

		/* now adjust the following entry */
		if (((void*)new_entry) + new_entry->size < manager.memory_end)
		{
			following_entry = ((void*)new_entry) + new_entry->size;
			following_entry->prev_entry = new_entry;
		}
			
		best_fit->size |= 0x01;	/* Set the allocated bit */

		result = (void*) ((void*)best_fit) + sizeof(MEMORY_HEADER);
	}

#if MEMORY_TEST > 0
	if (result != NULL)
	{
		/* find out who allocated this memory */
		new_entry = MM_MEMORY_HEADER(result);
		new_entry->alloc_by_line = __builtin_return_address(0);
#if MEMORY_TEST > 1
		new_entry->freed_by_line = NULL;
#endif		
	}
#endif

	sem_post(&manager.mutex);

	return result;
}


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  MemoryConcatenateForward
 *  Description:  This function will concatenate a memory entry with the next one
 *                in the chain.
 *-------------------------------------------------------------------------------------*/
void MemoryConcatenateForward (MEMORY_HEADER *header)
{
	MEMORY_HEADER	*after_after;
	MEMORY_HEADER	*following_entry = ((void*)header) + header->size;

	if (header->next_free == following_entry)
	{
		after_after = ((void*)following_entry) + following_entry->size;
		after_after->prev_entry = header;

		header->size += following_entry->size;
		header->next_free = following_entry->next_free;
	}
}

/*----- FUNCTION -----------------------------------------------------------------*
 * Name : FreeMemory
 * Desc : This function will free a lump of memory.
 *--------------------------------------------------------------------------------*/
void	FreeMemory(void* memory)
{
	MEMORY_HEADER	*free_space = manager.free_space.next_free;
	MEMORY_HEADER	*prev_free_space = &manager.free_space;
	MEMORY_HEADER	*following_entry;
	MEMORY_HEADER	*header;

	sem_wait(&manager.mutex);

	if (MM_POINTER_CHECK(memory))
	{
#if MEMORY_TEST == 5
		manager.num_frees++;
#endif		
		header = MM_MEMORY_HEADER(memory);

		/* check the allocated bit */
		if ((header->size & 0x01) == 0)
		{
			/* the allocation has been freed before */
#if MEMORY_TEST > 0
			printf("MEMORY: address %p has been freed before\n",memory);
			DumpSymbol(header->alloc_by_line,"alloc by");
#if MEMORY_TEST > 1
			DumpSymbol(header->freed_by_line,"freed by");
#if MEMORY_TEST == 4
			assert(duplicate_free);
#endif
#endif
#endif
		}
		else 
		{
			/* remove the allocated flag */
			header->size &= ~0x01;

			/* now do the free */
			if (header->prev_entry != NULL && ((header->prev_entry->size & 0x01) == 0))
			{
				/* the previous entry is free */
				header->prev_entry->size += header->size;

				/* now adjust the following entry */
				following_entry = ((void*)header) + header->size;

				if (((void*)following_entry) < manager.memory_end)
				{
					following_entry->prev_entry = header->prev_entry;
				}

				MemoryConcatenateForward(header->prev_entry);

				/* clear the element */
				header->size = 0;
				header->next_free  = NULL;
				header->prev_entry = NULL;
			}
			else
			{
				/* tedious search throughout the whole free list */
				while (free_space != NULL)
				{
					if (memory < ((void*)free_space))
					{
						/* we are before freespace */
						prev_free_space->next_free = header;
						header->next_free = free_space;
						break;
					}
					else if (free_space->next_free == NULL)
					{
						/* we are at the end of the list */
						free_space->next_free = header;
						header->next_free = NULL;
						break;
					}

					prev_free_space = free_space;
					free_space = free_space->next_free;
				}

				MemoryConcatenateForward(header);

				/* TODO: if free space == NULL then there is a problem */
				if (free_space == NULL)
				{
					printf("------------------ FAILED to free\n");
				}
			}

#if MEMORY_TEST > 1
			if (header != NULL)
			{
				/* find out who allocated this memory */
				header->freed_by_line = __builtin_return_address(0);
			}
#endif
		}
	}
#if MEMORY_TEST > 2
	else
	{
		DumpBacktrace(2,"NULL or Unaligned address passed to FreeMemory");
		
#if MEMORY_TEST == 4
		assert(bad_address);
#endif
	}
#endif

	sem_post(&manager.mutex);
}

#if 0

## Left out as it needs testing and I can't be bother today. Peter.

/*----- FUNCTION -----------------------------------------------------------------*
 * Name : ReAllocateMemory
 * Desc : This is a special for our re-alloc happy friends. 
 *        This function will try to efficiently re-allocate memory. It will try
 *        using the current page, then try the next block and finally do a full
 *        new allocation.
 *
 *        This function follows the C standard, if it cannot find the bigger
 *        block it returns NULL, if the new size is 0 it will free it and return
 *        NULL, if the size is smaller it will do the release in pages.
 *--------------------------------------------------------------------------------*/
void*	ReAllocateMemory(void* buffer, unsigned int size)
{
	void* 			result = NULL;
	unsigned int	required_size = (size + sizeof(MEMORY_HEADER) + MM_PAGE_BITMAP) & MM_PAGE_MASK;
	MEMORY_HEADER*	old_entry;
	MEMORY_HEADER*	current;

	if (MM_POINTER_CHECK(buffer))
	{
		old_entry = MM_MEMORY_HEADER(buffer);

		/* if it fits in the same allocation, do nothing */
		if (old_entry->size == required_size)
		{
			result = buffer;
		}
		else if ((old_entry->size & ~0x01) < required_size)
		{
			/* OK, the new allocation is smaller */
			/* TODO: handle this - we are unlikly to do this */
			printf("MEMORY: Allocation made smaller -- you need to handle this\n");
			result = buffer;
		}
		else
		{
			/* TODO: add an following element check */
			if ((result = AllocateMemory(size)) > 0)
			{
				/* we need to create a new allocation and move it */
				memcpy(result,buffer,old_entry->size);
				FreeMemory(buffer);
			}
		}
	}
#if MEMORY_TEST > 2
	else
	{
		DumpBacktrace(2,"NULL or Unaligned address passed to FreeMemory");
		
#if MEMORY_TEST == 4
		assert(bad_address);
#endif
	}
#endif

	return result;
}
#endif


#if MEMORY_TEST > 0
/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  DumpMemoryTable
 *  Description:  This function will dump the memory allocation table.
 *-------------------------------------------------------------------------------------*/
void DumpMemoryTable ( void )
{
	unsigned int	guard = 0;
	unsigned int	limit = 1000;
	unsigned int	total_free = 0;
	unsigned int	total_allocated = 0;
	unsigned char	string[100];
	MEMORY_HEADER	*free_space = manager.free_space.next_free;
	MEMORY_HEADER*	current = (void*)(((unsigned long)manager.memory + MM_PAGE_BITMAP) & MM_PAGE_MASK);
#if MEMORY_TEST > 2
	char**			strings;
	char*			dummy_array[1] = {NULL};
#else
	char*			strings[1] = {NULL};
#endif	

	sem_wait(&manager.mutex);

	printf( "MEMORY:\nMEMORY: Dump Memory Table\n"
			"MEMORY: index   current    memory_pointer size       allocated next       prev       free_link   who allocated\n"
			"MEMORY: ------- ---------- -------------- ---------- --------- ---------- ---------- ----------  -------------\n");

	while (((void*)current) < manager.memory_end && guard < limit && current->size != 0)
	{
#if MEMORY_TEST > 2
	if (current->alloc_by_line != NULL)
	{
		strings = backtrace_symbols(&current->alloc_by_line,1);	
	}
	else
	{
		strings = dummy_array;
	}
#endif	
		printf("MEMORY: [%5d] %p %p     %10d     %c     0x%08x 0x%08x 0x%08x %s\n",
				guard,
				current,
				(((void*)current) + sizeof(MEMORY_HEADER)),
				current->size & ~0x1,
				(current->size & 0x1)?'Y':'N',
				(unsigned int)((void*)(current)) + (current->size & ~0x01),
				(unsigned int)current->prev_entry,
				(unsigned int)current->next_free,
				(strings[0] == NULL)?"":strings[0]);

		if ((current->size & 0x01) == 1)
			total_allocated += current->size & ~0x01;
		else
			total_free += current->size & ~0x01;

		current = ((void*)current) + (current->size & ~0x01);
		guard++;
	}
	sem_post(&manager.mutex);

	printf(	"MEMORY: =========================================\n"
			"MEMORY: memory size: %d\n"
			"MEMORY:   allocated: %d\n"
			"MEMORY:        free: %d\n"
			"MEMORY: =========================================\n"
			,manager.size,total_allocated,total_free);
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  MemoryConsistancyCheck
 *  Description:  This function will check that the memory table is consistent.
 *-------------------------------------------------------------------------------------*/
int	MemoryConsistancyCheck(void)
{
	unsigned int	total = 0;
	MEMORY_HEADER*	current = (void*)(((unsigned long)manager.memory + MM_PAGE_BITMAP) & MM_PAGE_MASK);

	sem_wait(&manager.mutex);

	while (((void*)current) < manager.memory_end && current->size != 0)
	{
		total += current->size & ~0x01;
		current = ((void*)current) + (current->size & ~0x01);
	}

	sem_post(&manager.mutex);

	if (total == (((unsigned long)manager.size) & MM_PAGE_MASK) - 64)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
#endif
