/*-------------------------------------------------------------------------------------*
 *
 * name:  sem_test.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  08/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <semaphore.h>
#include <time.h>

void	main()
{
	int		fred = 0;
	sem_t	sem;
	struct	timespec timeout;

	sem_init(&sem,0,0);

	timeout.tv_sec = time(NULL) + 10;
	timeout.tv_nsec = 0;


	fred = sem_timedwait(&sem,&timeout);

	printf("fred: %d\n",fred);
}



/* $Id$ */

