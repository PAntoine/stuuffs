/*-------------------------------------------------------------------------------------*
 *
 * name:  stack_check.c
 * proj:  Miniweb browser version 3
 *
 * desc:  This function will check the depth of the stack.
 *
 * auth:  Peter Antoine  
 * date:  12/08/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

sem_t	sem;

__thread unsigned long	tls_base_stack = 0;
__thread unsigned long	tls_stack_size = 0;
__thread unsigned long	tls_thread_no  = 0;

unsigned long	array[10];

static void handler(int signum)
{
	printf("signal\n");
}

int	my_stack_function(unsigned int count, unsigned long stack_size)
{
	unsigned char	tools[300];
	unsigned int	number;

	if (tls_base_stack == 0L)
	{
		tls_base_stack = &number;
	}

//	printf("%d === stack addrs %p %p guess: %8luk address_calc: %8luk\n",count,tools,&number,(stack_size + 304)/1024,(((unsigned long)tls_base_stack - (unsigned long)tools)/1024));

	array[tls_thread_no] = (((unsigned long)tls_base_stack - (unsigned long)tools)/1024);

	number = my_stack_function(count,stack_size + 304);

	return number + 1;
}

void*	thread_function(void* id)
{
	unsigned char	start;
	tls_base_stack = (unsigned long) &start;

	tls_thread_no = (unsigned int) id;

	array[tls_thread_no] = 0;

	start += my_stack_function((unsigned int)id,0);

	return (void*) start;
}

void main()
{
	pthread_t		thread_1;
	pthread_t		thread_2;

    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; /* Restart functions if interrupted by handler */

    if (sigaction(SIGSEGV, &sa, NULL) == -1)
	{
		printf("FAILED to set the signal\n");
		exit(1);
	}

	pthread_create(&thread_1,NULL,thread_function,(void*)1);
	pthread_create(&thread_2,NULL,thread_function,(void*)2);

	while(1)
	{
		sleep(30);
	}
}

/* $Id$ */

