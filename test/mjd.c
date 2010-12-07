/*-------------------------------------------------------------------------------------*
 *
 * name:  mjd.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  19/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/
#include <time.h>

void	main()
{
	unsigned int	mjd;
	time_t			cur_time = time(NULL);
	struct tm*		temp_tm;
	struct tm		temp_tm2;



	temp_tm	= gmtime(&cur_time);

	if (temp_tm != NULL)
	{
		if (temp_tm->tm_mon < 2)
		{
			mjd = 14956 + temp_tm->tm_mday + (unsigned int)((temp_tm->tm_year + 1) * 365.25) + (unsigned int)((temp_tm->tm_mon + 2 + 12) * 30.6001);
		}
		else
		{
			mjd = 14956 + temp_tm->tm_mday + (unsigned int)((temp_tm->tm_year) * 365.25) + (unsigned int)((temp_tm->tm_mon + 2) * 30.6001);
		}
	}

	printf("%02x %02x %02x %02x\n",(mjd & 0xff),((mjd & 0xff00) >> 8),((mjd & 0xff0000) >> 16),((mjd & 0xff000000) >> 24));

	/* now will put the time in */
	printf("%01x%01x%01x%01x%01x%01x\n",
						temp_tm->tm_hour / 10,
						temp_tm->tm_hour % 10,
						temp_tm->tm_min / 10,
						temp_tm->tm_min % 10,
						temp_tm->tm_sec / 10,
						temp_tm->tm_sec % 10);


	printf("mjd: %d %p\n",mjd,mjd);
	
	memset(&temp_tm2,0,sizeof(struct tm));

	printf("%d \n",(int)((mjd - 15078.2)/365.25));
	printf("%d \n",(unsigned short)(mjd - 15078.2));

	/* now convert to posix time from MJD */
	temp_tm2.tm_year = (unsigned short)((mjd - 15078.2)/365.25);
	temp_tm2.tm_mon  = (unsigned short)((mjd - 14956.1 - ((unsigned int)(temp_tm2.tm_year * 365.25))) / 30.6001);

	temp_tm2.tm_mday = mjd - 14956 - (int)(temp_tm2.tm_year * 365.25) - (int)(temp_tm2.tm_mon * 30.6001);

	if (temp_tm2.tm_mon > 13)
		temp_tm2.tm_mon -= 14;
	else
		temp_tm2.tm_mon -= 2;


	printf("%2d/%02d/%02d (asctime: %s)\n",temp_tm2.tm_mday,temp_tm2.tm_mon,temp_tm2.tm_year,asctime(&temp_tm2));
}


/* $Id$ */

