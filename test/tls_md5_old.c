/*-------------------------------------------------------------------------------------*
 *
 * name:  test_md5.cpp
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  12/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

char*	test_array[] = {	"",
							"a",
							"abc",
							"message digest",
							"abcdefghijklmnopqrstuvwxyz",
							"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
							"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
							"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"};

void tls_MD5Init ( void );
void	tls_MD5Hash ( unsigned char* input_buffer, unsigned int input_size_bits, unsigned char* md5_hash );

void main()
{
	int				count;
	unsigned char	md5_hash[16];
	unsigned int 	data[256];

	tls_MD5Init();

	for (count = 0;count<8;count++)
	{
		tls_MD5Hash((unsigned char*)test_array[count],strlen(test_array[count])*8,md5_hash);

		printf("MD5: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",
				md5_hash[0],md5_hash[1],md5_hash[2],md5_hash[3],
				md5_hash[4],md5_hash[5],md5_hash[6],md5_hash[7],
				md5_hash[8],md5_hash[9],md5_hash[10],md5_hash[11],
				md5_hash[12],md5_hash[13],md5_hash[14],md5_hash[15]);
	}
}



/* $Id$ */

