#include <stdio.h>
#include <memory.h>


unsigned char	genration_array[] = "0123456789-.";
unsigned int	genration_size = sizeof(genration_array)-1;

int main()
{
	unsigned int	index;
	unsigned int	count;
	unsigned int	count_2;
	unsigned char	array[255];

	memset(array,0,255);

	for (count=0;count<genration_size;count++)
	{
		array[genration_array[count]] = 1;
	}

	printf("unsigned char	array[] =\n{\n");
	for (count=0,index=0;count<8;count++)
	{
		printf("	");
		for(count_2=0;count_2<16;count_2++,index++)
		{
			if (count_2 != 0)
				printf(",");

			if (array[index])
				printf("0x01");
			else
				printf("0x00");
		}

		if (count != 7)
			printf(",\n");
		else
			printf("\n");
	}
	printf("};");
}




