/*-------------------------------------------------------------------------------------*
 *
 * name:  bot_int_test.c
 * proj:  Miniweb browser version 3
 *
 * desc:  Testing the big int code.
 *
 * auth:  Peter Antoine  
 * date:  29/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include "PAPL_lib.h"

static unsigned char	test_string[512][64];
static unsigned int		index = 0;

static void genrate_bit_strings(void)
{
	unsigned int count = 0;
	unsigned int offset = 0;

	/* might as well start will all zeros */
	memset(test_string[index++],0x00,64);

	/* lets have some single bit patterns (byte) */
	for (count=0;count<8;count++,index++)
	{
		unsigned char byte = 0x01 << count;
		memset(test_string[index],byte,64);
	}

	/* single bit patterns (word) */
	for (count=0;count<16;count++,index++)
	{
		unsigned short word = 0x01 << count;

		for (offset=0;offset<64;offset+=2)
		{
			test_string[index][offset  ] = (word & 0xff00) >> 8;
			test_string[index][offset+1] = (word & 0x00ff);
		}
	}

	/* single bit patterns (long) */
	for (count=0;count<32;count++,index++)
	{
		unsigned long word = 0x01 << count;

		for (offset=0;offset<64;offset+=4)
		{
			test_string[index][offset  ] = (word & 0xff000000) >> 24;
			test_string[index][offset+1] = (word & 0x00ff0000) >> 16;
			test_string[index][offset+2] = (word & 0x0000ff00) >> 8;
			test_string[index][offset+3] = (word & 0x000000ff);
		}
	}

	/* lets do the whole 8 bit spectrum */
	for (count=0;count<256;count++,index++)
	{
		unsigned char byte = count;
		memset(test_string[index],byte,64);
	}

	/* now lets do the specials: 0x80...0x01 */
	test_string[index][0] = 0x80;
	memset(&test_string[index][1],0x00,62);
	test_string[index][63] = 0x01;
	index++;

	/* be wrong not to have all f's */
	memset(&test_string[index],0xff,64);
	index++;

	/* be wrong not to have all f's - 1 */
	memset(&test_string[index],0xff,64);
	test_string[index][63] = 0xfe;
	index++;

	/* be wrong not to have every bit except tp top bit set */
	memset(&test_string[index],0xff,64);
	test_string[index][0] = 0x7f;
	index++;


	/* would also be worth having 1 */
	memset(&test_string[index],0x00,63);
	test_string[index][63] = 0x01;
	index++;
}


int main()
{
	int count, vert;
	unsigned long	result[65];

	genrate_bit_strings();

	for (count=index-1;count>index-5;count--)
	{
		PAPL_MarshalInteger(64,test_string[count]);

		printf("%04d: ",count);
		for (vert=0;vert<64;vert++)
		{
			printf("%02x",test_string[count][vert]);
		}
		printf("\n");
	}

	memset((unsigned char*)result,0,65*4);

	PAPL_AddInteger(16,(unsigned long*)test_string[317],(unsigned long*)test_string[316],result);

	printf(" res: ");
	for (vert=0;vert<64;vert++)
	{
		printf("%02x",((unsigned char*) result)[vert]);
	}
	printf("\n");

	PAPL_AddInteger(16,(unsigned long*)test_string[317],(unsigned long*)test_string[8],result);

	printf(" res: ");
	for (vert=0;vert<64;vert++)
	{
		printf("%02x",((unsigned char*) result)[vert]);
	}
	printf("\n");


	PAPL_MarshalInteger(64,(unsigned char*)result);
	printf(" res: ");
	for (vert=0;vert<64;vert++)
	{
		printf("%02x",((unsigned char*) result)[vert]);
	}
	printf("\n");


}

/* $Id$ */

