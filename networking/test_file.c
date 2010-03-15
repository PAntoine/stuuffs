/*--------------------------------------------------------------------------------*
 * Name  : test_file.c
 * Desc  : 
 *
 * Author: peterantoine
 * Date  : 06/03/2010 18:01:02
 *--------------------------------------------------------------------------------*
 *                     Copyright (c) 2010 AntoineComputers
 *                            All rights Reserved.
 *--------------------------------------------------------------------------------*/
#include <stdio.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

void	find_names(void)
{
	int				err;
	int				sock;
	int				count = 0;
	char			names[80];
	struct hostent	*host_details;
	static struct ifreq ifr;
	static char *myip;
	static char *ifname = "wlan0";
	static struct ifreq ifreqs[20];
	struct ifconf ifconf;
	int  nifaces, i;
	unsigned long	net_mask = inet_addr("255.255.255.0");
	unsigned long	net_result;

	memset(&ifconf,0,sizeof(ifconf));
	ifconf.ifc_buf = (char*) (ifreqs);
	ifconf.ifc_len = sizeof(ifreqs);
	
	/* Get the interface IP address */

	sock = socket(AF_INET,SOCK_STREAM,0);

	strcpy( ifr.ifr_name, ifname );
	ifr.ifr_addr.sa_family = AF_INET;

	if ((err = ioctl( sock, SIOCGIFADDR, &ifr )) < 0)
		printf("error: %d\n",err);

	myip = inet_ntoa( ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr );
	printf("My IP: %s\n",myip);

	net_result = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr & net_mask;
	myip = inet_ntoa( net_result );

	printf("Masked: %s\n",myip);


	 if((err = ioctl(sock, SIOCGIFCONF , (char*) &ifconf  )) < 0 )
    	 perror("ioctl(SIOGIFCONF)");

	nifaces =  ifconf.ifc_len/sizeof(struct ifreq);
	 
	for(i = 0; i < nifaces; i++)
	{
		printf("\t%-10s\n", ifreqs[i].ifr_name);

//		if ((err = ioctl( sock, SIOCGIFADDR, &ifreqs[i] )) < 0)
//			printf("error: %d\n",err);

		printf("%p \n",((struct sockaddr_in *)&ifreqs[i].ifr_addr)->sin_addr);
		myip = inet_ntoa( ((struct sockaddr_in *)&ifreqs[i].ifr_addr)->sin_addr );
		printf("My IP: %s\n",myip);
	}

	if (gethostname(names,sizeof(names)) == -1)
	{
		printf("failed to get the host name\n");
	}
	else
	{
		host_details = gethostbyname(names);

		if (host_details != 0)
		{
			printf("host details: %p %d\n",host_details,count);

			while (host_details->h_aliases[count] != NULL)
			{
				printf("alias: %p %s \n",host_details->h_aliases[count],host_details->h_aliases[count]);
				count++;
			}

			count = 0;
			while (host_details->h_addr_list[count] != NULL)
			{
				struct in_addr	addr;

				memcpy(&addr,host_details->h_addr_list[count],sizeof(struct in_addr));

				printf("address: %s \n",inet_ntoa(addr));
				count++;
			}
		}
	}
}

int main()
{
	find_names();

	return 0;
}

