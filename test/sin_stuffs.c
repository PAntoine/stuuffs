/*-------------------------------------------------------------------------------------*
 *
 * name:  sin_stuffs.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  11/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <math.h>

static	unsigned long	md5_sin_table[64];

void	main()
{
	unsigned long	result;
	int		i;
	double	number;
	double	sin_value;

	for (i=0;i<64;i++)
	{
		sin_value = sin(i + 1);

		if (sin_value < 0)
			md5_sin_table[i] = (0 - sin_value) * 4294967296;
		else
			md5_sin_table[i] = sin_value * 4294967296;
	}

	for (i=0;i<64;i++)
	{
		if (sin(i) < 0)
			number = (0 - sin(i)) * 4294967296;
		else
			number = sin(i) * 4294967296;

		result = number;

		printf("[%d] %p %p\n",(int) i,md5_sin_table[i],result);
	}
}

/* $Id$ */

