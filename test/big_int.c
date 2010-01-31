/*-------------------------------------------------------------------------------------*
 *
 * name:  big_int.c
 * proj:  Miniweb browser version 3
 *
 * desc:  This is a test file handling large number arithmetic.
 *        Because this is aiming at encryption stuff's so all the arithmetic is aimed
 *        at that so all the numbers involved will be 512 and 1024 bits long.
 *
 *        Also I am not interested in singed arithmetic as all crypto is done on
 *        positive integers.
 *
 * auth:  Peter Antoine  
 * date:  28/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/



/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  PBIL_BigAdd
 *  Description:  This function is a simple big addition.
 *
 *                Obviously this function should be wrtting in ASM as then you can 
 *                use the CarryBit to handle this add without the shifting that has
 *                to be done with this 32bit C version.
 *
 *                And obviously if the numbers are already converted to long ints it
 *                makes the shifting easier and easier to read.
 *-------------------------------------------------------------------------------------*/
void PBIL_BigAdd ( unsigned int size, unsigned char number1, unsigned char number2, unsigned char result)
{
	unsigned int	count = 0;
	unsigned long	pd[8];

	unsigned char	*offset_1_1 = &number1[4 * 1];
	unsigned char	*offset_1_2 = &number1[4 * 2];
	unsigned char	*offset_1_3 = &number1[4 * 3];
	unsigned char	*offset_1_4 = &number1[4 * 4];
	unsigned char	*offset_1_5 = &number1[4 * 5];
	unsigned char	*offset_1_6 = &number1[4 * 6];
	unsigned char	*offset_1_7 = &number1[4 * 7];

	unsigned char	*offset_2_1 = &number2[4 * 1];
	unsigned char	*offset_2_2 = &number2[4 * 2];
	unsigned char	*offset_2_3 = &number2[4 * 3];
	unsigned char	*offset_2_4 = &number2[4 * 4];
	unsigned char	*offset_2_5 = &number2[4 * 5];
	unsigned char	*offset_2_6 = &number2[4 * 6];
	unsigned char	*offset_2_7 = &number2[4 * 7];


	while ((count + 8) < (size/4))
	{
		/* build the first addition source */
		li_1_1 = (((number1[count + 3] & 0x7f) << 24) | (number1[count + 2] << 16) | number1[count + 1] <<  8) | number1[count];
		li_1_2 = (((((offset_1_1[count + 3] & 0x3f) << 25) | (offset_1_1[count + 2] << 17) | offset_1_1[count + 1] <<  9) | offset_1_1[count] << 1) | (number1[count + 3] & 0x80) >> 7);
		li_1_3 = (((((offset_1_2[count + 3] & 0x1f) << 26) | (offset_1_2[count + 2] << 18) | offset_1_2[count + 1] << 10) | offset_1_2[count] << 2) | (number1[count + 3] & 0xc0) >> 6);
		li_1_4 = (((((offset_1_3[count + 3] & 0x0f) << 27) | (offset_1_3[count + 2] << 19) | offset_1_3[count + 1] << 11) | offset_1_3[count] << 3) | (number1[count + 3] & 0xe0) >> 5);
		li_1_5 = (((((offset_1_4[count + 3] & 0x07) << 28) | (offset_1_4[count + 2] << 20) | offset_1_4[count + 1] << 12) | offset_1_4[count] << 4) | (number1[count + 3] & 0xf0) >> 4);
		li_1_6 = (((((offset_1_5[count + 3] & 0x03) << 29) | (offset_1_5[count + 2] << 21) | offset_1_5[count + 1] << 13) | offset_1_5[count] << 5) | (number1[count + 3] & 0xf8) >> 3);
		li_1_7 = (((((offset_1_6[count + 3] & 0x01) << 30) | (offset_1_6[count + 2] << 22) | offset_1_6[count + 1] << 14) | offset_1_6[count] << 6) | (number1[count + 3] & 0xfc) >> 2);
		li_1_8 = (((                                         (offset_1_7[count + 2] << 23) | offset_1_7[count + 1] << 15) | offset_1_7[count] << 7) | (number1[count + 3] & 0xfe) >> 1);

		/* build the first addition source */
		li_2_1 = (((number2[count + 3] & 0x7f) << 24) | (number2[count + 2] << 16) | number2[count + 1] <<  8) | number2[count];
		li_2_2 = (((((offset_2_1[count + 3] & 0x3f) << 25) | (offset_2_1[count + 2] << 17) | offset_2_1[count + 1] <<  9) | offset_2_1[count] << 1) | (number2[count + 3] & 0x80) >> 7);
		li_2_3 = (((((offset_2_2[count + 3] & 0x1f) << 26) | (offset_2_2[count + 2] << 18) | offset_2_2[count + 1] << 10) | offset_2_2[count] << 2) | (number2[count + 3] & 0xc0) >> 6);
		li_2_4 = (((((offset_2_3[count + 3] & 0x0f) << 27) | (offset_2_3[count + 2] << 19) | offset_2_3[count + 1] << 11) | offset_2_3[count] << 3) | (number2[count + 3] & 0xe0) >> 5);
		li_2_5 = (((((offset_2_4[count + 3] & 0x07) << 28) | (offset_2_4[count + 2] << 20) | offset_2_4[count + 1] << 12) | offset_2_4[count] << 4) | (number2[count + 3] & 0xf0) >> 4);
		li_2_6 = (((((offset_2_5[count + 3] & 0x03) << 29) | (offset_2_5[count + 2] << 21) | offset_2_5[count + 1] << 13) | offset_2_5[count] << 5) | (number2[count + 3] & 0xf8) >> 3);
		li_2_7 = (((((offset_2_6[count + 3] & 0x01) << 30) | (offset_2_6[count + 2] << 22) | offset_2_6[count + 1] << 14) | offset_2_6[count] << 6) | (number2[count + 3] & 0xfc) >> 2);
		li_2_8 = (((                                         (offset_2_7[count + 2] << 23) | offset_2_7[count + 1] << 15) | offset_2_7[count] << 7) | (number2[count + 3] & 0xfe) >> 1);

		/* ok. Now do the actual additions */
		pd[0] = li_1_1 + li_2_1;
		pd[1] = li_1_2 + li_2_2;
		pd[2] = li_1_3 + li_2_3;
		pd[3] = li_1_4 + li_2_4;
		pd[4] = li_1_5 + li_2_5;
		pd[5] = li_1_6 + li_2_6;
		pd[6] = li_1_7 + li_2_7;
		pd[7] = li_1_8 + li_2_8;

		/* now the non-parallel bit */
		pd[1] = pd[1] + ((pd[0] & 0x80000000) >> 31);
		pd[2] = pd[2] + ((pd[1] & 0x80000000) >> 31);
		pd[3] = pd[3] + ((pd[2] & 0x80000000) >> 31);
		pd[4] = pd[4] + ((pd[3] & 0x80000000) >> 31);
		pd[5] = pd[5] + ((pd[4] & 0x80000000) >> 31);
		pd[6] = pd[6] + ((pd[5] & 0x80000000) >> 31);
		pd[7] = pd[7] + ((pd[6] & 0x80000000) >> 31);

		/* lets now output the result */
		for (index=0;index<8;index++)
		{
			/* TODO: need to shift it correctly by index for each round. */

		}

		/* now do the next one */
		count++;
	}

	carry = (res[(count * 8) - 1] & 0x80) >> 7;

	/* handle the remaining bytes */
	for (index = (count * 4);index < size;index++)
	{
		res = number1[index] + number2[index] + carry;
		out[index] = res & 0xFF;
		carry = (res & 0x100) >> 8;
	}

	/* dont lose the last carry */
	out[index] = (res & 0x100) >> 8;
}


/* $Id$ */

