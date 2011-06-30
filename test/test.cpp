/*-------------------------------------------------------------------------------------*
 *
 * name:  test.cpp
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

#ifndef BIG_ENDIAN
#define	WriteSize(x,y) {((unsigned long*)x)[0] = y;}
#else
#define	WriteSize(x,y) {(((unsigned char*)x)[0] = y & 0xff); ((unsigned char*)x)[1] = ((y & 0xff00) >> 8) ; ((unsigned char*)x)[2] = (y & 0xff0000 >> 16); (((unsigned char*)x)[3] = y & 0xff000000 >> 24);}
#endif

static unsigned long int data_mask[] = 
{
	0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F,
	0x000000FF, 0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF,
	0x0000FFFF, 0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF,
	0x00FFFFFF, 0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF
};

static unsigned long int data_bit[] = 
{
	0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
};

static unsigned long int empty_buffer[16];

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_MD5CopyBlock
 *  Description:  This function will copy the byte oriented data in the 32bit word 
 *                oriented data in the correct endianess.
 *
 *                Note: don't rely on the data pointer passed in, as the result may
 *                not be what you think.
 *-------------------------------------------------------------------------------------*/
unsigned long*	tls_MD5CopyBlock ( unsigned char* buffer, unsigned long int* md5_buffer )
{
#ifndef	BIG_ENDIAN
	return (unsigned long*) buffer;
#else
	for (int count=0;count<16;count++)
	{
		md5_buffer[count] = (((buffer[count*4  ])       |
							   buffer[count*4+1] << 8)  |
							   buffer[count*4+2] << 16) |
							   buffer[count*4+3] << 24;
	}

	memset((unsigned char*)empty_buffer,0,256);

	return md5_buffer;
#endif
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_MD5Hash
 *  Description:  This function will generate a MD5 Hash for the given value.
 *                NOTE: All data is handled little endian.
 *-------------------------------------------------------------------------------------*/
void	tls_MD5Hash ( unsigned char* input_buffer, unsigned long input_size_bits, unsigned long* md5_hash )
{
	unsigned int	input_size;
	unsigned int	padding_size;
	unsigned int	data_processed = 0;
	unsigned char	processing_section;
	unsigned long*	use_data;
	unsigned long	data_buffer[16];
	unsigned long	A = 0x67452301;
	unsigned long	B = 0xefcdab89;
	unsigned long	C = 0x98badcfe;
	unsigned long	D = 0x10325476;

	/* calc the number of padding bits */
	padding_size = 512 - ((input_size_bits + 64) % 512);
	input_size   = input_size_bits / 32;

	/* copy the next block of data that needs handling */
	while ((data_processed + 15) < input_size)
	{
		use_data = tls_MD5CopyBlock(&input_buffer[data_processed],data_buffer);
//		tls_ProcessBlock(&A,&B,&C,&D,use_data);
		data_processed += 16;
	}

	/* do the final block */
	memset((char*)data_buffer,0,16*4);
	memcpy((char*)data_buffer,&input_buffer[data_processed*4],(input_size - data_processed + 1)*4);

	data_buffer[(input_size - data_processed)] = (data_buffer[(input_size - data_processed)] & data_mask[input_size_bits % 32]) | data_bit[input_size_bits % 32];

	if ((input_size - data_processed) < 14)
	{
		WriteSize(&data_buffer[14],input_size_bits);
		data_buffer[15] = 0;
	}
		
//	tls_ProcessBlock(&A,&B,&C,&D,data_buffer);

	if ((input_size - data_processed) >= 14)
	{
		WriteSize(&empty_buffer[14],input_size_bits);
		empty_buffer[15] = 0;

//		tls_ProcessBlock(&A,&B,&C,&D,empty_buffer);
	}

	/* return the result */
	md5_hash[0] = A;
	md5_hash[1] = B;
	md5_hash[2] = C;
	md5_hash[3] = D;
}
 
int main()
{
	unsigned int  q;
	unsigned char data[1024];
	unsigned long result[4];

	for (q=439;q<613;q++)
	{
		memset(data,0,1024);

		for (int i=0;i<q;i++)
		{
			data[(i/8)] |= 1 << (i % 8);
		}

		tls_MD5Hash(data,q,result);
	}
}

/* $Id$ */

