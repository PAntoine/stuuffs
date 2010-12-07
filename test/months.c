/*-------------------------------------------------------------------------------------*
 *
 * name:  months.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  22/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/
unsigned int month[2][12] 		= {{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};
unsigned int offset_month[][12]	= {{31,31,28,31,30,31,30,31,31,30,31,30},{31,31,29,31,30,31,30,31,31,30,31,30}};
unsigned int month_day[2][12] 	= {{0,31,59,90,120,151,181,212,243,273,304,334},{0,31,60,91,121,152,182,213,244,274,305,335}};
unsigned int month_start[][12]	= {{0,3,3,6,1,4,6,2,5,0,3,5},{6,2,3,6,1,4,6,2,5,0,3,5}};

unsigned char* month_name[] = {
"Janurary",
"February",
"March",
"April",
"May",
"June",
"July",
"August",
"September",
"October",
"November",
"December"
};

unsigned char* days[] = {"sun","mon","tue","wed","thu","fri","sat"};


int	main()
{
	int count;
	unsigned int day;
	unsigned int year = 2010;
	unsigned int y = year % 100;

	unsigned int start_of_week = 1;
	
	printf("start of century: %d\n",(2 * (3 - ((year / 100) & 0x3))));
	printf("days offset for year: %d\n",(y + y/4));

	for (count=0;count<12;count++)
	{
		day = ((2 * (3 - ((year / 100) & 0x3))) + (y + y/4) + month_start[0][count] + 1) % 7;

		printf("%s :",month_name[count]);

		if (day == start_of_week)
		{
			printf("starts the start of week\n");

		}
		else
		{
			printf(" start %s prev_end %s ",days[day],days[(6+day)%7]);
			printf("mday %d\n",offset_month[0][count] - ((6+day-start_of_week)%7));
		}
	}


}





/* $Id$ */

