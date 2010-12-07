/*-------------------------------------------------------------------------------------*
 *
 * name:  alarms.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  21/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>

#define	MAX_ALARMS	(10)

typedef struct
{
	time_t			alarm_time;
	unsigned int	app_no;
	unsigned int	value;
	unsigned int	next_alarm;

} WAITING_ALARMS_LIST;

typedef struct
{
	time_t				next_alarm_time;
	unsigned int		first_alarm;
	unsigned int		num_pending;
	unsigned int		free_alarms;
	WAITING_ALARMS_LIST	details[MAX_ALARMS];

} ALARMS_LIST;

static ALARMS_LIST	list;

void	dump_list(void)
{
	int count;
	for (count=0;count<MAX_ALARMS;count++)
	{
		printf("[%d] {%d} %08d %3d %3d\n",
				count,
				list.details[count].next_alarm,
				list.details[count].alarm_time,
				list.details[count].app_no,
				list.details[count].value);
	}
}

static void handler(int signum)
{
	printf("in alarm handler\n");
	next_alarm();
}

int next_alarm(void)
{
	int				result = 0;
	time_t			cur_time = time(NULL);
	unsigned int	hold;

	if (list.first_alarm != MAX_ALARMS)
	{
		printf("%08d %3d %3d\n",
				list.details[list.first_alarm].alarm_time,
				list.details[list.first_alarm].app_no,
				list.details[list.first_alarm].value);
	}

	do
	{
		if (list.first_alarm != MAX_ALARMS)
		{
			hold = list.first_alarm;
			list.first_alarm = list.details[hold].next_alarm;
			
			list.details[hold].next_alarm = list.free_alarms;
			list.free_alarms = hold;
		}
	}
	while (list.first_alarm != MAX_ALARMS && (cur_time > list.details[list.first_alarm].alarm_time));

	if (list.first_alarm != MAX_ALARMS)
	{
		/* set the next alarm */
		printf("setting: alarm - expired\n");
		alarm(list.details[list.first_alarm].alarm_time - cur_time);
		result = 1;
	}

	return result;
}

int	add_alarm(time_t alarm_time, unsigned int app_no, unsigned int value)
{
	int				result = -1;
	time_t			cur_time = time(NULL);
	unsigned int	new_alarm;
	unsigned int	cur_alarm;
	unsigned int	prev_alarm;

	if (alarm_time < cur_time)
	{
		printf("timer expired: %d %d\n",cur_time,alarm_time);
	}
	else if ((new_alarm = list.free_alarms) < MAX_ALARMS)
	{
		list.free_alarms = list.details[new_alarm].next_alarm;
		list.details[new_alarm].next_alarm = MAX_ALARMS;

		list.details[new_alarm].value = value;
		list.details[new_alarm].app_no = app_no;
		list.details[new_alarm].alarm_time = alarm_time;

		if (list.first_alarm == MAX_ALARMS)
		{
			list.first_alarm = new_alarm;

			/* first alarm */
			printf("add: setting alarm - first\n");
			alarm(alarm_time - cur_alarm);
		}
		else if (list.details[list.first_alarm].alarm_time > alarm_time)
		{
			/* add to the front */
			list.details[new_alarm].next_alarm = list.first_alarm;
			list.first_alarm = new_alarm;

			/* need to usurp the current alarm */
			printf("add: setting alarm - add to front\n");
			alarm(alarm_time - cur_time);
		}
		else
		{
			printf("add in the middle\n");
			/* find it in the list */
			prev_alarm = list.first_alarm;
			cur_alarm  = list.details[prev_alarm].next_alarm;

			while(cur_alarm < MAX_ALARMS)
			{
				if (list.details[cur_alarm].alarm_time > alarm_time)
				{
					list.details[new_alarm].next_alarm = cur_alarm;
					break;
				}

				prev_alarm = cur_alarm;
				cur_alarm = list.details[cur_alarm].next_alarm;
			}
					
			list.details[prev_alarm].next_alarm = new_alarm;
		}
		result = new_alarm;
	}

	return result;
}

void	main()
{
	sem_t				fw;
	struct sigaction	sa;
	time_t				cur_time = time(NULL);
	unsigned int		count;

	sem_init(&fw,0,0);

	/* set up the ALARM handler */
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGALRM,&sa,NULL);

	/* now set up the alarm array */
	memset(&list,0,sizeof(ALARMS_LIST));

	for (count=0;count<MAX_ALARMS;count++)
	{
		list.details[count].next_alarm = count+1;
	}

	list.free_alarms = 0;
	list.first_alarm = MAX_ALARMS;
	list.next_alarm_time = UINT_MAX;

	/* set test alarm */
	add_alarm(cur_time + 10,1,300);
	add_alarm(cur_time + 15,2,500);
	add_alarm(cur_time + 5 ,3,100);
	add_alarm(cur_time + 6 ,4,200);
	add_alarm(cur_time + 11,5,400);

	dump_list();

	/* stall the code for the events */
	sem_wait(&fw);

	printf("still working\n");
}


/* $Id$ */

