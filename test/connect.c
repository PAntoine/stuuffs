/*-------------------------------------------------------------------------------------*
 *
 * name:  connect.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  05/07/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int sd = socket(AF_INET, SOCK_STREAM, 0);  /* init socket descriptor */
	struct sockaddr_in sin;
	int len;

	/*** PLACE DATA IN sockaddr_in struct ***/
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);

	/*** CONNECT SOCKET TO THE SERVICE DESCRIBED BY sockaddr_in struct ***/
	if (connect(sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("connecting");
		exit(1);
	}else{
		printf("connected ok\n");
	}
}

/* $Id$ */

