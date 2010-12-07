/*-------------------------------------------------------------------------------------*
 *
 * name:  ir.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  13/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define	START_BITS		(0x7)
#define ADDRESS_BITS	(0x1F << 3)
#define COMMAND_BITS	(0x3F)
#define I_COMMAND_BITS	(0xF6)

unsigned int	ir_table[] = {	0x0052,0x0090,0x00a0,0x00ca,0x00f0,0x00c8,0x0012,0x008a,0x00d2,0x0060,0x0050,0x00c2,0x0080,0x0040,0x0072,0x004a,
								0x00da,0x0058,0x001a,0x00e2,0x0022,0x00b0,0x0070,0x002a,0x000a,0x0032,0x0068,0x00fa,0x0078,0x003a,0x00b8,0x00d8,
								0x0082,0x00c0,0x0042,0x00a2,0x00e0,0x0062,0x00aa,0x00e8,0x006a,0x0052,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};

unsigned int	ir_lookup[] = {
								0x002f,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x0018,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x00ff,0x00ff,0x0006,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x0012,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x00ff,0x00ff,0x0014,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x0017,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x00ff,0x00ff,0x0019,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x001d,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x000d,0x00ff,0x0022,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x000f,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x000a,0x00ff,0x0029,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x0011,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x0009,0x00ff,0x0025,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x001a,0x00ff,0x0028,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x0016,0x00ff,0x000e,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x001c,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x000c,0x00ff,0x0020,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x0007,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x0001,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x0002,0x00ff,0x0023,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x0026,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x0015,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x001e,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x0021,0x00ff,0x000b,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x0005,0x00ff,0x0003,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x00ff,0x00ff,0x0008,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x001f,0x00ff,0x0010,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x0024,0x00ff,0x0013,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x0027,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,
								0x0004,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff,0x001b,0x00ff,0x00ff,0x00ff,0x00ff,0x00ff
							};


char* strings[] = {
	"MWKV_POWER_KEY",
	"MWKV_TV_KEY",
	"MWKV_OPTION_KEY",
	"MWKV_GUIDE_KEY",
	"MWKV_MENU_KEY",
	"MWKV_INTERACTIVE_KEY",
	"MWKV_SERVICES_KEY",
	"MWKV_MUTE_KEY",
	"MWKV_INFORMATION_KEY",
	"MWKV_UP_KEY",
	"MWKV_DOWN_KEY",
	"MWKV_LEFT_KEY",
	"MWKV_RIGHT_KEY",
	"MWKV_SELECT_KEY",
	"MWKV_VOL_UP_KEY",
	"MWKV_VOL_DOWN_KEY",
	"MWKV_CHAN_UP_KEY",
	"MWKV_CHAN_DOWN_KEY",
	"MWKV_TEXT_KEY",
	"MWKV_BACK_KEY",
	"MWKV_HELP_KEY",
	"MWKV_FASTFORWARD_KEY",
	"MWKV_REWIND_KEY",
	"MWKV_PAUSE_KEY",
	"MWKV_PLAY_KEY",
	"MWKV_STOP_KEY",
	"MWKV_RECORD_KEY",
	"MWKV_COLOR_KEY_1",
	"MWKV_COLOR_KEY_2",
	"MWKV_COLOR_KEY_3",
	"MWKV_COLOR_KEY_4",
	"MWKV_0_KEY",
	"MWKV_1_KEY",
	"MWKV_2_KEY",
	"MWKV_3_KEY",
	"MWKV_4_KEY",
	"MWKV_5_KEY",
	"MWKV_6_KEY",
	"MWKV_7_KEY",
	"MWKV_8_KEY",
	"MWKV_9_KEY"};



unsigned int convert_nec (unsigned char* keycode)
{
	unsigned char	high_byte = keycode[2];
	unsigned char	low_byte  = keycode[3];
	unsigned char	swapped;
	unsigned char	fred = ~high_byte;

		if (fred != low_byte)
	{
		printf("failed\n");
		swapped = 0;
	}else{
		/* really want to write this in asm as could do clever things with ASL and ROT - *sigh* */
		swapped = 	(high_byte >> 7) |
					((high_byte & 0x40) >> 5) |
					((high_byte & 0x20) >> 3) |
					((high_byte & 0x10) >> 1) |
					((high_byte & 0x08) << 1) |
					((high_byte & 0x04) << 3) |
					((high_byte & 0x02) << 5) |
					((high_byte & 0x01) << 7);
	}

	return swapped;
}

void	main()
{
	int 	ir_handle = open("/dev/ir",O_RDONLY);
	unsigned int	num_key_pressed = 0;
	unsigned char	key_codes[256];
	unsigned char	key_pressed[256];
	unsigned long	char_buffer;
	union {
		unsigned long l;
		unsigned char c[sizeof(unsigned long)];
	} u;

		unsigned int y;
		unsigned int array[256];

		printf("after atfer\n");

		memset(array,0xff,sizeof(array));

		printf("after atfer\n");
		for (y=0;y<41;y++)
		{
			printf("%d: %d\n",y,ir_table[y]);
			array[ir_table[y]] = y;
		}

		for (y=0;y<256;y++)
		{
			if (array[y] < 48)
			{
				printf("%s,",strings[array[y]]);
			}
			else
			{
				printf("MW_INVALID_KEY,");
			}

			if (((y+1) % 8) == 0)
				printf("\n");

		}


	memset(key_pressed,0,256);


	if (ir_handle < 0)
	{
		printf("ERROR: failed to open the ir driver\n");
	}
	else
	{
		while(1)
		{
#if 0
			{
					fd_set rfds;
					struct timeval tv;
					int ret;
		
					FD_ZERO(&rfds);
					FD_SET(ir_handle, &rfds);
					tv.tv_sec = 5;
					tv.tv_usec = 0;

//					ret = select(ir_handle + 1, &rfds, NULL, NULL, &tv);
//					if (ret) {
						if (read(ir_handle, &u.l, sizeof(unsigned long)) > 0)
						{
							printf("stuff: 0x%08lx %ld\n", u.l,convert_nec(u.l));
						}
						else
							printf("stuff: nothing\n");
//					} else {
//						printf("stuff: timeout\n");
//					}
				}
#endif
			read(ir_handle,&char_buffer,sizeof(unsigned long));
			u.l = char_buffer;

			printf("%c%c%c%c%c%c%c%c ",
					(u.c[0] & 0x01)?'1':'0',
					(u.c[0] & 0x02)?'1':'0',
					(u.c[0] & 0x04)?'1':'0',
					(u.c[0] & 0x08)?'1':'0',
					(u.c[0] & 0x10)?'1':'0',
					(u.c[0] & 0x20)?'1':'0',
					(u.c[0] & 0x40)?'1':'0',
					(u.c[0] & 0x80)?'1':'0');

			printf("%c%c%c%c%c%c%c%c ",
					(u.c[1] & 0x01)?'1':'0',
					(u.c[1] & 0x02)?'1':'0',
					(u.c[1] & 0x04)?'1':'0',
					(u.c[1] & 0x08)?'1':'0',
					(u.c[1] & 0x10)?'1':'0',
					(u.c[1] & 0x20)?'1':'0',
					(u.c[1] & 0x40)?'1':'0',
					(u.c[1] & 0x80)?'1':'0');
			printf("%c%c%c%c%c%c%c%c ",
					(u.c[2] & 0x01)?'1':'0',
					(u.c[2] & 0x02)?'1':'0',
					(u.c[2] & 0x04)?'1':'0',
					(u.c[2] & 0x08)?'1':'0',
					(u.c[2] & 0x10)?'1':'0',
					(u.c[2] & 0x20)?'1':'0',
					(u.c[2] & 0x40)?'1':'0',
					(u.c[2] & 0x80)?'1':'0');
			printf("%c%c%c%c%c%c%c%c ",
					(u.c[3] & 0x01)?'1':'0',
					(u.c[3] & 0x02)?'1':'0',
					(u.c[3] & 0x04)?'1':'0',
					(u.c[3] & 0x08)?'1':'0',
					(u.c[3] & 0x10)?'1':'0',
					(u.c[3] & 0x20)?'1':'0',
					(u.c[3] & 0x40)?'1':'0',
					(u.c[3] & 0x80)?'1':'0');


			printf("%08x %d %s\n",char_buffer,convert_nec(u.c),strings[array[convert_nec(u.c)]]);

			key_codes[num_key_pressed++] = convert_nec(u.c);

			if (key_pressed[convert_nec(u.c)] == 1)
			{
				if (convert_nec(u.c) == 82)
				{
					int x,count;
					for (count=0;count<num_key_pressed;)
					{
						for(x=count;count<x+16;count++)
						{
							printf("0x%02x,",key_codes[count]);
						}
						printf("\n");
					}

					printf("reverse lookup\n");
					for (count=0;count<256;)
					{
						for(x=count;count<x+16;count++)
						{
							printf("0x%02x,",(unsigned char)array[count]);
						}
						printf("\n");
					}


				}
				else
				{
					printf("dup: %d\n",convert_nec(u.c));
					printf("start again :)\n");
				}
			}
			else
				key_pressed[convert_nec(u.c)] = 1;
		}
	}
}




/* $Id$ */

