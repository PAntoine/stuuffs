/*-------------------------------------------------------------------------------------*
 *
 * name:  PAPL_lib.h
 * proj:  Miniweb browser version 3
 *
 * desc:  Peters Arbitrary Precision Library
 *        This is a library of functions that handle arbitrary precision maths.
 *
 * auth:  Peter Antoine  
 * date:  29/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#ifndef	__PAPL_LIBRARY_H__
#define	__PAPL_LIBRARY_H__

/*-----------------------------------------------------------------------------*
 *  PAPL error codes.
 *-----------------------------------------------------------------------------*/

typedef enum
{
	PAPLE_OK,
	PAPLE_SIZE_NOT_DWORD_ALIGNED

} PAPL_ERROR;

/*-----------------------------------------------------------------------------*
 *  PAPL function definitions.
 *-----------------------------------------------------------------------------*/
int PAPL_isBigEndian(void);
PAPL_ERROR	PAPL_MarshalInteger ( unsigned int byte_length, unsigned char* source);
void PAPL_AddInteger ( unsigned dword_length, unsigned long* a, unsigned long* b, unsigned long* result );

#endif

/* $Id$ */

