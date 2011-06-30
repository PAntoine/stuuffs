/***************************************************************************************
 *
 *       Filename:  tls_MD5Hash
 *
 *    Description:  This file holds the functions required to handle a MD5 hash. 
 *
 *        Version:  1.0
 *        Created:  18/03/2009 08:37:35
 *       Revision:  none
 *
 *         Author:  Peter Antoine
 *          Email:  me@peterantoine.me.uk
 *
 *------------------------------------------------------------------------------------- 
 *                         Copyright (c) 2009 : Peter Antoine
 *                        Released under the Artistic License.
 ***************************************************************************************/

//#include "http_server.h"
#include <math.h>
#include <stdio.h>
#include <memory.h>

/*-----------------------------------------------------------------------------*
 * function definitions.
 * F, G, H and I are basic MD5 functions.
 *-----------------------------------------------------------------------------*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#ifndef BIG_ENDIAN
#define	WriteSize(x,y) {((unsigned int*)x)[0] = (unsigned int)y;}
#else
#define	WriteSize(x,y) {(((unsigned char*)x)[0] = y & 0xff); ((unsigned char*)x)[1] = ((y & 0xff00) >> 8) ; ((unsigned char*)x)[2] = (y & 0xff0000 >> 16); (((unsigned char*)x)[3] = y & 0xff000000 >> 24);}
#endif

/*-----------------------------------------------------------------------------*
 * static definitions.
 *-----------------------------------------------------------------------------*/
static unsigned int data_mask[] = 
{
	0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F,
	0x000000FF, 0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF,
	0x0000FFFF, 0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF,
	0x00FFFFFF, 0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF
};

static unsigned int data_bit[] = 
{
	0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001, 
	0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
	0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
	0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000
};

static unsigned int empty_buffer[16];

static	unsigned char	md5_padding[64] = {	0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
											0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
											0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
											0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static	unsigned int	md5_sin_table[65];

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_MD5Init
 *  Description:  This function will initialise the MD5 sin table.
 *-------------------------------------------------------------------------------------*/

void tls_MD5Init ( void )
{
	int	i;
	double	sin_value;

	for (i=0;i<65;i++)
	{
		sin_value = sin((double)i);

		/* ABS does not do doubles in C */
		if (sin_value < 0)
			md5_sin_table[i] = (0 - sin_value) * 4294967296;
		else
			md5_sin_table[i] = sin_value * 4294967296;
	}
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_MD5Round1
 *  Description:  This function is the MD5 round 1 function.
 *-------------------------------------------------------------------------------------*/
unsigned int tls_MD5Round1 ( unsigned int a, unsigned int b, unsigned int c, unsigned int d , unsigned int k, unsigned int s, unsigned int i, unsigned int* data)
{
	return (b + ROTATE_LEFT((a + F(b,c,d) + data[k] + md5_sin_table[i]),s)); 
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_MD5Round2
 *  Description:  This function is the MD5 round 2 function.
 *-------------------------------------------------------------------------------------*/
unsigned int tls_MD5Round2 ( unsigned int a, unsigned int b, unsigned int c, unsigned int d , unsigned int k, unsigned int s, unsigned int i, unsigned int* data)
{
	return (b + ROTATE_LEFT((a + G(b,c,d) + data[k] + md5_sin_table[i]),s)); 
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_MD5Roundr3
 *  Description:  This function is the MD5 round 3 function.
 *-------------------------------------------------------------------------------------*/
unsigned int tls_MD5Round3 ( unsigned int a, unsigned int b, unsigned int c, unsigned int d , unsigned int k, unsigned int s, unsigned int i, unsigned int* data)
{
	return (b + ROTATE_LEFT((a + H(b,c,d) + data[k] + md5_sin_table[i]),s)); 
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_MD5Round4
 *  Description:  This function is the MD5 round 4 function.
 *-------------------------------------------------------------------------------------*/
unsigned int tls_MD5Round4 ( unsigned int a, unsigned int b, unsigned int c, unsigned int d , unsigned int k, unsigned int s, unsigned int i, unsigned int* data)
{
	return (b+ ROTATE_LEFT((a + I(b,c,d) + data[k] + md5_sin_table[i]),s)); 
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_ProcessBlock
 *  Description:  This function actually holds the MD5 sum function. It works accross
 *                a specific block of data. It updates the 128bit MD register.
 *-------------------------------------------------------------------------------------*/
void tls_ProcessBlock ( unsigned int* A, unsigned int* B,unsigned int* C,unsigned int* D, unsigned int* data)
{
	unsigned int	AA = *A;
	unsigned int	BB = *B;
	unsigned int	CC = *C;
	unsigned int	DD = *D;

	/* do round 1 */
	*A = tls_MD5Round1(*A,*B,*C,*D ,0 ,7 ,1,data);
	*D = tls_MD5Round1(*D,*A,*B,*C ,1,12 ,2,data);
	*C = tls_MD5Round1(*C,*D,*A,*B ,2,17 ,3,data);
	*B = tls_MD5Round1(*B,*C,*D,*A ,3,22 ,4,data);
	*A = tls_MD5Round1(*A,*B,*C,*D ,4 ,7 ,5,data);
	*D = tls_MD5Round1(*D,*A,*B,*C ,5,12 ,6,data);
	*C = tls_MD5Round1(*C,*D,*A,*B ,6,17 ,7,data);
	*B = tls_MD5Round1(*B,*C,*D,*A ,7,22 ,8,data);
	*A = tls_MD5Round1(*A,*B,*C,*D ,8 ,7 ,9,data);
	*D = tls_MD5Round1(*D,*A,*B,*C ,9,12,10,data);
	*C = tls_MD5Round1(*C,*D,*A,*B,10,17,11,data);
	*B = tls_MD5Round1(*B,*C,*D,*A,11,22,12,data);
	*A = tls_MD5Round1(*A,*B,*C,*D,12 ,7,13,data);
	*D = tls_MD5Round1(*D,*A,*B,*C,13,12,14,data);
	*C = tls_MD5Round1(*C,*D,*A,*B,14,17,15,data);
	*B = tls_MD5Round1(*B,*C,*D,*A,15,22,16,data);

	/* do round 2 */
	*A = tls_MD5Round2(*A,*B,*C,*D, 1, 5,17,data);
	*D = tls_MD5Round2(*D,*A,*B,*C, 6, 9,18,data);
	*C = tls_MD5Round2(*C,*D,*A,*B,11,14,19,data);
	*B = tls_MD5Round2(*B,*C,*D,*A, 0,20,20,data);
	*A = tls_MD5Round2(*A,*B,*C,*D, 5, 5,21,data);
	*D = tls_MD5Round2(*D,*A,*B,*C,10, 9,22,data);
	*C = tls_MD5Round2(*C,*D,*A,*B,15,14,23,data);
	*B = tls_MD5Round2(*B,*C,*D,*A, 4,20,24,data);
	*A = tls_MD5Round2(*A,*B,*C,*D, 9, 5,25,data);
	*D = tls_MD5Round2(*D,*A,*B,*C,14, 9,26,data);
	*C = tls_MD5Round2(*C,*D,*A,*B, 3,14,27,data);
	*B = tls_MD5Round2(*B,*C,*D,*A, 8,20,28,data);
	*A = tls_MD5Round2(*A,*B,*C,*D,13, 5,29,data);
	*D = tls_MD5Round2(*D,*A,*B,*C, 2, 9,30,data);
	*C = tls_MD5Round2(*C,*D,*A,*B, 7,14,31,data);
	*B = tls_MD5Round2(*B,*C,*D,*A,12,20,32,data);

	/* do round 3 */
	*A = tls_MD5Round3(*A,*B,*C,*D, 5, 4,33,data);
	*D = tls_MD5Round3(*D,*A,*B,*C, 8,11,34,data);
	*C = tls_MD5Round3(*C,*D,*A,*B,11,16,35,data);
	*B = tls_MD5Round3(*B,*C,*D,*A,14,23,36,data);
	*A = tls_MD5Round3(*A,*B,*C,*D, 1, 4,37,data);
	*D = tls_MD5Round3(*D,*A,*B,*C, 4,11,38,data);
	*C = tls_MD5Round3(*C,*D,*A,*B, 7,16,39,data);
	*B = tls_MD5Round3(*B,*C,*D,*A,10,23,40,data);
	*A = tls_MD5Round3(*A,*B,*C,*D,13, 4,41,data);
	*D = tls_MD5Round3(*D,*A,*B,*C, 0,11,42,data);
	*C = tls_MD5Round3(*C,*D,*A,*B, 3,16,43,data);
	*B = tls_MD5Round3(*B,*C,*D,*A, 6,23,44,data);
	*A = tls_MD5Round3(*A,*B,*C,*D, 9, 4,45,data);
	*D = tls_MD5Round3(*D,*A,*B,*C,12,11,46,data);
	*C = tls_MD5Round3(*C,*D,*A,*B,15,16,47,data);
	*B = tls_MD5Round3(*B,*C,*D,*A, 2,23,48,data);

	/* do round 4 */
	*A = tls_MD5Round4(*A,*B,*C,*D ,0 ,6,49,data);
	*D = tls_MD5Round4(*D,*A,*B,*C ,7,10,50,data);
	*C = tls_MD5Round4(*C,*D,*A,*B,14,15,51,data);
	*B = tls_MD5Round4(*B,*C,*D,*A ,5,21,52,data);
	*A = tls_MD5Round4(*A,*B,*C,*D,12 ,6,53,data);
	*D = tls_MD5Round4(*D,*A,*B,*C ,3,10,54,data);
	*C = tls_MD5Round4(*C,*D,*A,*B,10,15,55,data);
	*B = tls_MD5Round4(*B,*C,*D,*A ,1,21,56,data);
	*A = tls_MD5Round4(*A,*B,*C,*D ,8 ,6,57,data);
	*D = tls_MD5Round4(*D,*A,*B,*C,15,10,58,data);
	*C = tls_MD5Round4(*C,*D,*A,*B ,6,15,59,data);
	*B = tls_MD5Round4(*B,*C,*D,*A,13,21,60,data);
	*A = tls_MD5Round4(*A,*B,*C,*D ,4 ,6,61,data);
	*D = tls_MD5Round4(*D,*A,*B,*C,11,10,62,data);
	*C = tls_MD5Round4(*C,*D,*A,*B ,2,15,63,data);
	*B = tls_MD5Round4(*B,*C,*D,*A ,9,21,64,data);

	/* end the loop */
	*A += AA;
	*B += BB;
	*C += CC;
	*D += DD;
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  tls_MD5CopyBlock
 *  Description:  This function will copy the byte oriented data in the 32bit word 
 *                oriented data in the correct endianess.
 *
 *                Note: don't rely on the data pointer passed in, as the result may
 *                not be what you think.
 *-------------------------------------------------------------------------------------*/
unsigned int*	tls_MD5CopyBlock ( unsigned char* buffer, unsigned int* md5_buffer )
{
#ifndef	BIG_ENDIAN
	return (unsigned int*) buffer;
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
void	tls_MD5Hash ( unsigned char* input_buffer, unsigned int input_size_bits, unsigned char* md5_hash )
{
	unsigned int	input_size;
	unsigned int	data_processed = 0;
	unsigned int*	use_data;
	unsigned int	data_buffer[16];
	unsigned int	A = 0x67452301;
	unsigned int	B = 0xefcdab89;
	unsigned int	C = 0x98badcfe;
	unsigned int	D = 0x10325476;

	/* calc the number of bytes to process */
	input_size   = input_size_bits / 32;

	/* copy the next block of data that needs handling */
	while ((data_processed + 15) < input_size)
	{
		use_data = tls_MD5CopyBlock(&input_buffer[data_processed*4],data_buffer);
		tls_ProcessBlock(&A,&B,&C,&D,use_data);
		data_processed += 16;
	}

	/* do the final block */
	memset((char*)data_buffer,0,16*4);
	use_data = tls_MD5CopyBlock(&input_buffer[data_processed*4],data_buffer);
	memcpy((char*)data_buffer,(char*)use_data,(input_size - data_processed + 1)*4);

	data_buffer[(input_size - data_processed)] = (data_buffer[(input_size - data_processed)] & data_mask[input_size_bits % 32]) | data_bit[input_size_bits % 32];

	if ((input_size - data_processed) < 14)
	{
		WriteSize(&data_buffer[14],input_size_bits);
		data_buffer[14] = (unsigned int) input_size_bits;
		data_buffer[15] = 0;
	}
	
	tls_ProcessBlock(&A,&B,&C,&D,data_buffer);

	if ((input_size - data_processed) >= 14)
	{
		WriteSize(&empty_buffer[14],input_size_bits);
		empty_buffer[15] = 0;

		tls_ProcessBlock(&A,&B,&C,&D,empty_buffer);
	}

	/* return the result */
	md5_hash[ 0] = A & 0xFF;
	md5_hash[ 1] = ((A & 0xFF00) >> 8);
	md5_hash[ 2] = ((A & 0xFF0000) >> 16);
	md5_hash[ 3] = ((A & 0xFF000000) >> 24);

	md5_hash[ 4] = B & 0xFF;
	md5_hash[ 5] = ((B & 0xFF00) >> 8);
	md5_hash[ 6] = ((B & 0xFF0000) >> 16);
	md5_hash[ 7] = ((B & 0xFF000000) >> 24);

	md5_hash[ 8] = C & 0xFF;
	md5_hash[ 9] = ((C & 0xFF00) >> 8);
	md5_hash[10] = ((C & 0xFF0000) >> 16);
	md5_hash[11] = ((C & 0xFF000000) >> 24);

	md5_hash[12] = D & 0xFF;
	md5_hash[13] = ((D & 0xFF00) >> 8);
	md5_hash[14] = ((D & 0xFF0000) >> 16);
	md5_hash[15] = ((D & 0xFF000000) >> 24);
}

