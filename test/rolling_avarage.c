/*-------------------------------------------------------------------------------------*
 *
 * name:  rolling_avarage.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  02/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

unsigned int	calc_rolling_avarage(unsigned int avarage,unsigned int data, unsigned int num_points)
{
	return (avarage + (((int)data - (int)avarage) / (int) num_points));
}


void	main()
{
	unsigned int ava = 0,count;
	unsigned int points[] = {10,2,4,5,4,5,6,7,8,8,9,9,9,5,6,7,8,9,9,0,6,5,4,33,2,5,7,7,7};
	unsigned int size = sizeof(points)/sizeof(points[0]);

	for (count=0;count<size;count++)
	{
		ava = calc_rolling_avarage(ava,points[count],count+1);
		printf("ava: %d\n",ava);
	}

	printf("rolling = %d\n",ava);
	ava = 0;
	for (count=0;count<size;count++)
	{
		ava += points[count];
	}

	ava = ava/size;
	printf("std = %d\n",ava);
}

/* $Id$ */

