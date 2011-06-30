/*--------------------------------------------------------------------------------*
 *                          HomeNet Project
 *  _     _                      ______
 * | |   | |                    |  ___ \       _
 * | |__ | | ___  ____   ____   | |   | | ____| |_
 * |  __)| |/ _ \|    \ / _  )  | |   | |/ _  )  _)
 * | |   | | |_| | | | ( (/ /   | |   | ( (/ /| |__ 
 * |_|   |_|\___/|_|_|_|\____)  |_|   |_|\____)\___)
 *
 * Name  : time_stuff
 * Desc  : General time testing stuff.
 *
 * Author: peterantoine
 * Date  : 01/05/2010 14:55:17
 *--------------------------------------------------------------------------------*
 *                     Copyright (c) 2010 AntoineComputers
 *                              Public Domain.
 *--------------------------------------------------------------------------------*/
#include <time.h>
#include <stdio.h>

unsigned char	HNUT_DayOfWeek(unsigned short year, unsigned char month, unsigned char mday)
{
	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	year -= month < 3;
	return (year + year/4 - year/100 + year/400 + t[month-1] + mday) % 7;
}

void	main()
{
	struct tm	time_struct;
	struct tm	time_struct_2;
	time_t		time_value;
	unsigned int months[] = {31,28,31,30,31,30,31,31,30,31,30,31};

	time_value = time(NULL);

	printf("time: %d\n",time_value);
	memcpy(&time_struct,gmtime(&time_value),sizeof(struct tm));
	printf("time: %s\n",asctime(&time_struct));

	memset(&time_struct_2,0,sizeof(struct tm));
	time_struct_2.tm_year = time_struct.tm_year;
	time_struct_2.tm_mday = 1;

	time_value = mktime(&time_struct_2);
	memcpy(&time_struct,gmtime(&time_value),sizeof(struct tm));
	printf("time: %s (day of week: %d)\n",asctime(&time_struct),time_struct.tm_wday);

	printf("start day of year: %d\n",HNUT_DayOfWeek(2010,1,1));

	{
		int count;
		int index = 0;
		for (count = 0;count<12;count++)
		{
			printf("%d,",index);
			index += months[count];
		}
		printf("\n");
	}


}

