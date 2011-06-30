#include <unistd.h>
#include <memory.h>
#include <malloc.h>

void	InitialiseMemory(void* memory_start, unsigned int memory_size);
void*	AllocateMemory(unsigned int size);
void	FreeMemory(void* memory);
void*	ReAllocateMemory(void* buffer, unsigned int size);

int 	main()
{
	unsigned int	failed_count = 0;
	unsigned int	count;
	void*			memory;
	char*			alloc[1000];

	memory = malloc(1 * 1024 * 1024);

	InitialiseMemory(memory,(1*1024*1024));

		if (!MemoryConsistancyCheck())
		{
			printf("after init\n");
			DumpMemoryTable();
			exit(1);
		}

	DumpMemoryTable();

	for (count=1;count < 1000; count++)
	{
		if ((alloc[count] = AllocateMemory(1024)) == NULL)
		{
			failed_count++;
		}
	}
		if (!MemoryConsistancyCheck())
		{
			DumpMemoryTable();
			exit(1);
		}


	printf("Failed to allocate %d requests\n",failed_count);

	MemoryConsistancyCheck();
	printf("free 200\n");
	FreeMemory(alloc[200]);
	MemoryConsistancyCheck();
	printf("free 202\n");
	FreeMemory(alloc[202]);
	MemoryConsistancyCheck();
	printf("free 201\n");
	FreeMemory(alloc[201]);
	MemoryConsistancyCheck();


	printf("before the next set deletes\n");
	for (count=1;count < 10 /*00 - failed_count*/; count++)
	{
		FreeMemory(alloc[count]);
		alloc[count] = NULL;
		printf("free %d\n",count);
		MemoryConsistancyCheck();
	}

	for (count=400;count > 300 /*00 - failed_count*/; count--)
	{
		FreeMemory(alloc[count]);
		alloc[count] = NULL;
	}

	if (!MemoryConsistancyCheck())
	{
		DumpMemoryTable();
		exit(1);
	}

	DumpMemoryTable();

	{
		int i;
	for (i=0,count=100;count<400;count += 17)
	{
		printf("[%d] count: %d\n",i++,count);
		if (AllocateMemory(count) == NULL)
		{
			printf("second round of allocations: size %d\n",count);
		}
	}
	DumpMemoryTable();
	}

	/* free them again */
	for (count=1;count < 1000 - failed_count; count++)
	{
		if (alloc[count] != NULL)
			FreeMemory(alloc[count]);

		if (!MemoryConsistancyCheck())
		{
			printf("count: %d\n",count);
			DumpMemoryTable();
			exit(1);
		}
	}

	DumpMemoryTable();

	DumpStatistics();
}

