/***************************************************************************************
 *
 *       Filename:  test_read.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/03/2010 21:11:03
 *       Revision:  none
 *
 *         Author:  Peter Antoine
 *          Email:  me@peterantoine.me.uk
 *
 *------------------------------------------------------------------------------------- 
 *                         Copyright (c) 2010 : Peter Antoine
 *                        Released under the Artistic License.
 ***************************************************************************************/

#include <winsock2.h>

void	hexDump ( const unsigned char* memory, unsigned int dumpSize)
{
	static char	HexBit[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	int			nibble,count,count2,count3,count4;
	char		line[81];

	line[80] = '\0';

	memset(line,' ',0);

	/* write the straplines */
	printf("             Hex Dump                                          Characters      \n");
	printf("             ------------------------------------------------  ----------------\n");

	/* clear the space under the timestamp */
	memset(line,' ',80);
	
	/* now dump the rest */
	for (count=0;count < dumpSize;count+=16)
	{
//		line[0] = '0';
//		line[1] = 'x';
//		line[2] = HexBit[(((((unsigned long)memory) + count) & 0xf0000000) >> 28)];
//		line[3] = HexBit[(((((unsigned long)memory) + count) & 0x0f000000) >> 24)];
//		line[4] = HexBit[(((((unsigned long)memory) + count) & 0x00f00000) >> 20)];
//		line[5] = HexBit[(((((unsigned long)memory) + count) & 0x000f0000) >> 16)];
//		line[6] = HexBit[(((((unsigned long)memory) + count) & 0x0000f000) >> 12)];
//		line[7] = HexBit[(((((unsigned long)memory) + count) & 0x00000f00) >> 8)];
//		line[8] = HexBit[(((((unsigned long)memory) + count) & 0x000000f0) >> 4)];
//		line[9] = HexBit[(((((unsigned long)memory) + count) & 0x0000000f))];
//
		for(count2=count,count3=13,count4=63;count2 < count+16 && count2 < dumpSize;count2++,count3+=3,count4++)
		{
			/* do the main hex conversion */
			line[count3] = HexBit[((memory[count2]&0xf0)>>4)];
			line[count3+1] = HexBit[(memory[count2]&0x0f)];
			line[count3+2] = ' ';

			/* do the end stuff */
			if (!isprint(memory[count2]))
				line[count4] = '.';
			else
				line[count4] = (char) memory[count2];
		}

		if (count2 < count+16)
		{
			for(;count2<count+16;count2++,count3+=3,count4++)
			{
				line[count3] = ' ';
				line[count3+1] = ' ';
				line[count3+2] = ' ';

				line[count4] = ' ';
			}
		}

		line[79] = '\n';
		printf("%s",line);
	}
}



int main()
{
	int					s_in;
	int					len = sizeof(struct sockaddr_in);
	int					read_len;
	char				buffer[1024];
	WSADATA				wsaData;
	struct sockaddr_in	from;
	struct sockaddr_in	sin_addr;

	WSAStartup(MAKEWORD(2,2), &wsaData);

	s_in = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	memset(&sin_addr,0,sizeof(struct sockaddr_in));
	sin_addr.sin_family = AF_INET;
	sin_addr.sin_addr.s_addr = INADDR_ANY;
	sin_addr.sin_port = htons(27015);

	bind(s_in,(struct sockaddr*)&sin_addr,sizeof(sin_addr));

	while(1)
	{
		read_len = recvfrom(s_in,buffer,1024,0,(struct sockaddr*)&from,&len);
		printf("read len: %d\n",read_len);
		hexDump(buffer,read_len);
	}

	return 0;
}

