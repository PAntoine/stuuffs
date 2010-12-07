/*-------------------------------------------------------------------------------------*
 *
 * name:  double_check.cpp
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  23/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/
#include <stdio.h>

int	main()
{
	int		qwer = 1;
	double	fred(12);
	int		qway = 5;
	double 	harry = 1.0;
	int 	aqwe = 6;
	int		count = 0;

	printf("sizeof: %d\n",sizeof(harry));

	for (count=0;count<10;count++)
	{
		printf("%d %d %d %d %f %f\n",qwer,qway,aqwe,count*fred,fred,harry);

		harry = fred / count;
		fred = fred + 0.3;
	}

	return 0;
}



/* $Id$ */

