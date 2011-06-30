/*-------------------------------------------------------------------------------------*
 *
 * name:  block_test.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  07/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void	main()
{
	int	index= 0;
	int	count,qount;
	int	fd  = open("/dev/sda",O_RDONLY);
	unsigned char	buffer[1024];

	memset(buffer,0,1024);
	for (count=0;count<64;count++)
	{
		for (qount=0;qount<16;index++,qount++)
			printf("%02x ",buffer[index]);
		printf("\n");
	}

	index=  0;

	if (read(fd,buffer,1024) == 1024)
	{
		printf("worked \n");
	}
	else
	{
		printf("failed \n");
	}

	for (count=0;count<64;count++)
	{
		for (qount=0;qount<16;index++,qount++)
			printf("%02x ",buffer[index]);
		printf("\n");
	}

	close(fd);
}





/* $Id$ */

