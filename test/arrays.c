/*-------------------------------------------------------------------------------------*
 *
 * name:  arrays.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  03/09/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

typedef struct
{
	unsigned int	one;
	unsigned int	two;
	unsigned char	three[2];
} ITEM_STRUCT;

ITEM_STRUCT	first = {1,2,"w"};
ITEM_STRUCT	second = {1,3,"a"};
ITEM_STRUCT	third = {1,4,"s"};

ITEM_STRUCT*	array[] = {&first,&second,&third};

int	main()
{
	printf("third: %d %s\n",array[2]->one,array[2]->three);

	return 0;
}


/* $Id$ */

