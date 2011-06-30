/*--------------------------------------------------------------------------------*
 *                          HomeNet Project
 *  _     _                      ______
 * | |   | |                    |  ___ \       _
 * | |__ | | ___  ____   ____   | |   | | ____| |_
 * |  __)| |/ _ \|    \ / _  )  | |   | |/ _  )  _)
 * | |   | | |_| | | | ( (/ /   | |   | ( (/ /| |__ 
 * |_|   |_|\___/|_|_|_|\____)  |_|   |_|\____)\___)
 *
 * Name  : offsetof
 * Desc  : Does offsetof do what I think it should.
 *
 * Author: peterantoine
 * Date  : 02/05/2010 15:28:53
 *--------------------------------------------------------------------------------*
 *                     Copyright (c) 2010 AntoineComputers
 *                            All rights Reserved.
 *--------------------------------------------------------------------------------*/
#include <stddef.h>


typedef struct
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	array[200];
} STRUCT_1;

typedef struct
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	z;
	unsigned int	array[200];
} STRUCT_2;

typedef struct
{
	unsigned int	fred;
	union
	{
		STRUCT_1	a;
		STRUCT_2	b;
	} data;
} STRUCT_3;

typedef struct
{
	unsigned int	a;
	unsigned char	b;
	unsigned char	c;
	STRUCT_1		d;
	STRUCT_3		e;
} STRUCT_4;

int	main()
{
	int	fred = offsetof(STRUCT_4,e.data.b.array[4]);

	printf("fred: %d\n",fred);

	return 0;
}

