/*-------------------------------------------------------------------------------------*
 *
 * name:  backtrace.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  01/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/
#include <execinfo.h>
#include <unistd.h>

void	bt_func(void)
{
	int size;
	void*	buffer[2];
	char**	strings;

	size = backtrace(buffer,2);
	strings = backtrace_symbols(buffer,size);

	if (strings == NULL)
	{
		printf("failed\n");
	}
	else
	{
		printf("size: %d %s %s\n",size,strings[0],strings[1]);
	}

	free(strings);
}

void my_func3(void)
{
	bt_func();
}

void my_func2(void)
{
	bt_func();

	my_func3();
}

void my_func(void)
{
	bt_func();

	my_func2();
}

void main()
{
	my_func();

	bt_func();
}


/* $Id$ */

