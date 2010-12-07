/*-------------------------------------------------------------------------------------*
 *
 * name:  inher_test.cpp
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  28/09/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <stdio.h>

class base
{
public:
	base();
	virtual	bool	fred() = 0;
protected:
	bool	m_field;
};

class sub_class : public base
{
	public:
		sub_class();
};

base::base()
{
	m_field = true;
	printf("base_class\n");
}

sub_class::sub_class()
{
	m_field = true;
	printf("sub_class\n");
}

int	main()
{
	sub_class	one;
	sub_class*	two = new sub_class();
}








/* $Id$ */

