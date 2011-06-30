/*-------------------------------------------------------------------------------------*
 *
 * name:  bit_int_2.c
 * proj:  Miniweb browser version 3
 *
 * desc:  Second go for defining a library of functions that handle arbitrary precision
 *        maths.
 *
 * auth:  Peter Antoine  
 * date:  29/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include "PAPL_lib.h"

typedef union
{
	unsigned char	test[4];
	unsigned long	test_num;
} PAPL_ENDIAN_TEST;

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  PAPL_isBigEndian
 *  Description:  This function will test to see if the processor is big or little
 *                endian.
 *-------------------------------------------------------------------------------------*/
int PAPL_isBigEndian(void)
{
	int	result;
	PAPL_ENDIAN_TEST	test;

	test.test_num = 0x04030201;

	if (test.test[0] == 0x01)
	{
		result = 0;
	} 
	else if (test.test[0] != 0x04)
	{
		result = 1;
	}

	return result;
}


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  PAPL_MarshalInteger
 *  Description:  It makes sense before any actual mathematics takes place that the
 *                integer numbers are in the endian order to make the rest of the
 *                calculations easier.
 *
 *                Note: this works on the way in as on the way out.
 *
 *					TODO: This bollocks. Need to make it little endian in dwords.
 *
 *-------------------------------------------------------------------------------------*/
PAPL_ERROR	PAPL_MarshalInteger ( unsigned int byte_length, unsigned char* source)
{
	unsigned int	count;
	unsigned char	temp[4];
	PAPL_ERROR		result = PAPLE_OK;

	if ((byte_length & 0x03) != 0)
	{
		result = PAPLE_SIZE_NOT_DWORD_ALIGNED;
	}
	else
	{
		if (!PAPL_isBigEndian())
		{
			for (count=0;count<byte_length;count+=4)
			{
				temp[0] = source[count + 0];
				temp[1] = source[count + 1];
				temp[2] = source[count + 2];
				temp[3] = source[count + 3];

				source[count + 0] = temp[3];
				source[count + 1] = temp[2];
				source[count + 2] = temp[1];
				source[count + 3] = temp[0];
			}
		}
	}

	return result;
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  PAPL_AddInteger
 *  Description:  This function will handle a long integer addition.
 *                It uses the following to handle carry propagation.
 *
 *                      c = ((a ^ b) & (s ^ 1))
 *
 *                Sorry this algo does not parallelise but I do embedded so I only
 *                have one core :D.
 *-------------------------------------------------------------------------------------*/

void PAPL_AddInteger ( unsigned dword_length, unsigned long* a, unsigned long* b, unsigned long* result )
{
	unsigned int carry = 0;
	unsigned int count = dword_length;

	do
	{
		count--;
		result[count] = a[count] + b[count] + carry;
		carry = (((a[count] ^ b[count]) & (result[count] ^ 0x80000000)) >> 31);
	}
	while(count != 0);
}

/* $Id$ */

