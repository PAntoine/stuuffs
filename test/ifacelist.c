/*
 * ifacelist.c
 * Author: neo@techpulp.com
 * Techpulp Technologies
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/route.h>

void	hexdump(void* data,int size)
{
	int i;
	for (i=0;i<size;i++)
		printf("%02x ",((unsigned char*)data)[i]);

	printf("\n");
}

int get_iface_list(struct ifconf *ifconf)
{
   int sock, rval;

   sock = socket(AF_INET,SOCK_STREAM,0);
   if(sock < 0)
   {
     perror("socket");
     return (-1);
   }

   if((rval = ioctl(sock, SIOCGIFCONF, (char*) ifconf  )) < 0 )
     perror("ioctl(SIOGIFCONF)");

   close(sock);

   return rval;
}


int main()
{
   static struct ifreq ifreqs[20];
   struct ifconf ifconf;
   int  nifaces, i;
   struct ifaddrs *ifap,*next;
   struct ifreq		ifhwreq;
   int sock, rval, nroutes;
   struct rtentry	rentry[20];


   memset(&ifconf,0,sizeof(ifconf));
   ifconf.ifc_buf = (char*) (ifreqs);
   ifconf.ifc_len = sizeof(ifreqs);

   sock = socket(AF_INET,SOCK_STREAM,0);
   if(sock < 0)
   {
     perror("socket");
     return (-1);
   }
#if 0
	/* read the routing table */
	route_list.rtc_len = sizeof(rentry);
	route_list.rtc_configured = 0;
	route_list.rtc_returned = 0;
	route_list.rtcu_buf = (char*) rentry;

	if (ioctl(sock,SIOGRTCONF,(char*) &route_list) < 0)
	{
		perror("Failed to get route list");
	}
	else
	{
		nroutes = route_list.len / sizeof(struct rtentry);

		printf("number of routes are: %d\n",nroutes);
	}
#endif

	/* gbet some stuff */
   if((rval = ioctl(sock, SIOCGIFCONF, (char*) &ifconf  )) < 0 )
     perror("ioctl(SIOGIFCONF)");

   nifaces =  ifconf.ifc_len/sizeof(struct ifreq);

   printf("Interfaces (count = %d):\n", nifaces);
   for(i = 0; i < nifaces; i++)
   {
	   printf("\t%-10s ", ifreqs[i].ifr_name);
		strncpy(ifhwreq.ifr_name, ifreqs[i].ifr_name,sizeof(ifhwreq.ifr_name));
		hexdump(&((struct sockaddr_in)(ifreqs[i].ifr_addr)).sin_addr.s_addr,4);

		printf("%s\n",ifhwreq.ifr_name);

		if( ioctl(sock, SIOCGIFHWADDR, (char*) &ifhwreq) >= 0 )
		{
			hexdump(&ifhwreq.ifr_hwaddr.sa_data,5);
		}

	   hexdump(&ifreqs[i],sizeof(struct ifreq));
   }

	getifaddrs(&ifap);

	next = ifap;
	while (next != NULL)
	{
		if (next->ifa_addr->sa_family == AF_INET)
		{
			printf("%s ",next->ifa_name);
			printf("%p ",((struct sockaddr_in*)next->ifa_addr)->sin_addr.s_addr);
			printf("%p\n",((struct sockaddr_in*)next->ifa_netmask)->sin_addr.s_addr);
		}

		next = next->ifa_next;
	}

	freeifaddrs(ifap);
	close (sock);
}

