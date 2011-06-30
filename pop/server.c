#include "pop3.h"
#include <winsock2.h>

#define POP3_PORT	(110)

static char	HexBit[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/*-----------------------------------------------------------------------------*
 *  The standard strings that are required for the POP3 client.
 *-----------------------------------------------------------------------------*/
static unsigned char	error[] = "-ERR go away\x0d\x0a";
static unsigned char	dot[] = ".\x0d\x0a";

static unsigned char	greeting[] = "+OK POP3 server ready <0000123434556@here.com>\x0d\x0a";
static unsigned char	ok[] = "+OK\x0d\x0a";
static unsigned char	huh_response[] = "+\x0d\x0a";
static unsigned char	capa_sasl_list[] = "+OK\x0d\x0aSASL CRAM-MD5 DIGEST-MD5\x0d\x0aUIDL\x0d\x0a.\x0d\x0a";

static unsigned int		ok_size = sizeof(ok)-1;
static unsigned int		capa_sasl_size = sizeof(capa_sasl_list)-1;
static unsigned int		error_size = sizeof(error)-1;
static unsigned int		huh_response_size = sizeof(huh_response)-1;
static unsigned int		greeting_size = sizeof(greeting)-1;

extern char	*pop_command_strings[];

extern POP3_NAMES	auth_types[];
extern unsigned int auth_size;

/* TODO: Testing */
char test_message[] = 	"From: John Doe <jdoe@machine.example>\x0d\x0a"
						"To: Mary Smith <mary@example.net>\x0d\x0a"
						"Subject: Saying Hello\x0d\x0a"
						"Date: Fri, 21 Nov 1997 09:55:06 -0600\x0d\x0a"
						"Message-ID: <1234@local.machine.example>\x0d\x0a"
						"\x0d\x0a"
						"This is a message just to say hello.\x0d\x0a"
						"So, \"Hello\".\x0d\x0a.\x0d\x0a";


/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  DumpHexMem
 *  Description:  This function will dump the memory supplied as hex. 
 *-------------------------------------------------------------------------------------*/
void	DumpHexMem(char* memory,unsigned long dumpSize)
{
	int	count,count2,count3,count4;
	char	line[89];

	/* write the straplines */
	printf("                     Hex Dump                                          Characters      \n");
	printf("                     ------------------------------------------------  ----------------\n");

	/* clear the space under the timestamp */
	memset(line,' ',86);
	
	/* now dump the rest */
	for (count=0;count < dumpSize;count+=16)
	{
		for(count2=count,count3=21,count4=71;count2 < count+16 && count2 < dumpSize;count2++,count3+=3,count4++)
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

		line[87] = '\n';
		line[88] = '\0';
		printf("%s",line);
	}
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  POP3_GetToken
 *  Description:  This is simple find the space and return the size token finder.
 *-------------------------------------------------------------------------------------*/

int	POP3_GetToken ( char* buffer, char* token, int token_size)
{
	int	result = 0;

	while (result < token_size && buffer[result] != ' ' && buffer[result] != 0x0d && buffer[result] != '\0')
	{
		token[result] = buffer[result];
		result++;
	}

	token[result] = '\0';

	return result;
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  POP3_sendGreeting
 *  Description:  This function will send the basic POP greeting.
 *                TODO: This function should also generate the connection nonce and 
 *                      all the nonsense that will be required later.
 *-------------------------------------------------------------------------------------*/

void POP3_sendGreeting ( POP3_CONNECTION *connection )
{
	send(connection->socket,greeting,greeting_size,0);
	connection->mode = POP3_AUTHENTICATION_MODE;
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  POP3_DecodeCommand
 *  Description:  This function will decode the POP3 command.
 *-------------------------------------------------------------------------------------*/

POP3_COMMANDS	POP3_DecodeCommand (POP3_CONNECTION *connection )
{
	unsigned int	count;
	POP3_COMMANDS	result = POP3C_INVALID_COMMAND;

	if (connection->data_size < 4)
	{
		result = POP3C_INVALID_COMMAND;
	}
	else
	{
		DumpHexMem(connection->buffer,connection->data_size);

		/* OH, forgive the GOD of good coding :) */
		if (connection->buffer[3] == ' ' && (((int*)(pop_command_strings[0]))[0] == ((int*)connection->buffer)[0]))
		{
			result = POP3C_TOP;
		}
		else
		{
			for (count=0;count<POP3C_MAX_COMMANDS;count++)
			{
				if (((int*)(pop_command_strings[count]))[0] == ((int*)(connection->buffer))[0])
				{
					result = count;
					break;
				}
			}

			printf("result: %d\n",result);

			/* TODO: Check the mode of the email server before allowing the command 
			 * -- Better still add MODE to the command name listing - use the one for auth_types 
			 */
			switch(result)
			{
				/* these commands are not implemented yet */
				case POP3C_TOP:
				case POP3C_APOP:
					result = POP3C_INVALID_COMMAND;
					break;

				/* these message are implemented */
				case POP3C_USER:
				case POP3C_PASS:
				case POP3C_RSET:
				case POP3C_NOOP:
				case POP3C_DELE:
				case POP3C_RETR:
				case POP3C_UIDL:
				case POP3C_LIST:
				case POP3C_STAT:
				case POP3C_AUTH:
				case POP3C_CAPA:
					break;

				case POP3C_QUIT:
					connection->mode = POP3_UPDATE_MODE;
					result = POP3C_QUIT;
					break;

				default:
					result = POP3C_INVALID_COMMAND;
			}
		}
	}

	connection->command = result;

	return result;
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  POP3_HandleConnection
 *  Description:  This function will handle the request command.
 *-------------------------------------------------------------------------------------*/
POP3_COMMANDS	POP3_HandleConnection (POP3_CONNECTION *connection )
{
	int				bytes_read;
	unsigned int	count;
	unsigned int	length;
	POP3_COMMANDS	result;
	char			token[32];

	printf("%d %d %d\n",connection->command,POP3C_AUTH,POP3C_CAPA);

	switch(connection->command)
	{
		case POP3C_USER:
			/* TODO: look up the user name before accepting */
			send(connection->socket,error,error_size,0);
			break;

		case POP3C_PASS:
			/* TODO: look up the user name before accepting - and check the pasword */
			send(connection->socket,ok,ok_size,0);
			connection->mode = POP3_TRANSACTION_MODE;
			break;

		case POP3C_NOOP:
			send(connection->socket,ok,ok_size,0);
			break;

		case POP3C_RSET:
			/* TODO: This function so reset any deletes that are scheduled */
			send(connection->socket,ok,ok_size,0);
			break;

		case POP3C_DELE:
			length = POP3_GetToken(&connection->buffer[5],token,32);

			printf("it wants to delete: %s\n",token);
			send(connection->socket,ok,ok_size,0);
			break;

		case POP3C_RETR:
			length = POP3_GetToken(&connection->buffer[5],token,32);

			printf("it wants message number: %s\n",token);
			send(connection->socket,ok,ok_size,0);
			send(connection->socket,test_message,sizeof(test_message)-1,0);
			break;

		case POP3C_UIDL:
			if (connection->buffer[4] == 0x0d)
			{
				printf("just the uinque IDs\n");
				/* just a plain LIST ALL command */
				send(connection->socket,"+OK\x0d\x0a""1 THIRD\x0d\x0a.\x0d\x0a",sizeof("+OK\x0d\x0a""1 FIRST\x0d\x0a.\x0d\x0a")-1,0);

			}
			else if ( (length = POP3_GetToken(&connection->buffer[5],token,32)) > 0)
			{
				/* ok have a request for a single item */

			}
			else
			{
				send(connection->socket,error,error_size,0);
			}
			break;

		case POP3C_LIST:
			if (connection->buffer[4] == 0x0d)
			{
				printf("just the list\n");
				/* just a plain LIST ALL command */
				send(connection->socket,"+OK\x0d\x0a""1 200\x0d\x0a.\x0d\x0a",sizeof("+OK\x0d\x0a""1 200\x0d\x0a.\x0d\x0a")-1,0);

			}
			else if ( (length = POP3_GetToken(&connection->buffer[5],token,32)) > 0)
			{
				/* ok have a request for a single item */

			}
			else
			{
				send(connection->socket,error,error_size,0);
			}
			break;


		case POP3C_STAT:
			/* its asking me for the number of messages I have */
			/* TODO: Look at the DB */
			send(connection->socket,"+OK 1 200\x0d\x0a",sizeof("+OK 1 200\x0d\x0a")-1,0);
			break;

		case POP3C_AUTH:
			if (connection->buffer[4] == 0x0d)
			{
				printf("it is looking for the list of AUTH codes that we support\n");
				/* just a plain LIST ALL command */
				send(connection->socket,"+OK\x0d\x0a""CRAM-MD5\x0d\x0a""DIGEST-MD5\x0d\x0a.\x0d\x0a",sizeof("+OK\x0d\x0a""CRAM-MD5\x0d\x0a""DIGEST-MD5\x0d\x0a.\x0d\x0a")-1,0);

			}
			else
			{
				length = POP3_GetToken(&connection->buffer[5],token,32);

				printf("GetToken: %s %s %d\n",&connection->buffer,token,auth_size);
				for (count=0;count<auth_size;count++)
				{
					printf("length:%d %d\n",length,auth_types[count].length);
					printf("names: %s %s\n",token,auth_types[count].name);
					if (length == auth_types[count].length && memcmp(token,auth_types[count].name,length) == 0)
					{
						printf("not here\n");
						/*OK. we have an auth type that we understand */
						connection->auth_type = auth_types[count].value;
						break;
					}
				}

				switch(connection->auth_type)
				{
					case POP3A_PLAIN:

						printf("plain: %02x\n",connection->buffer[4 + length]);

						if (connection->buffer[5 + length] == 0x0d)
						{
							/* ok we dont have the initial challenge so send the "huh?" response */
							send(connection->socket,huh_response,huh_response_size,0);
							connection->waiting_for_data = 1;
						}
						else
						{
							/* we have the user name now */
							/* TODO: set the user name */
							connection->mode = POP3_TRANSACTION_MODE;
							send(connection->socket,ok,ok_size,0);
						}
						break;

					case POP3A_CRAM_MD5:
						/* TODO: this needs a proper seed */
						printf("cram-md5: %02x\n",connection->buffer[4 + length]);
						send(connection->socket,"+ 54354354352345342534545432456789\x0d\x0a",sizeof("+ 54354354352345342534545432456789\x0d\x0a")-1,0);
						connection->waiting_for_data = 1;

						break;

					default:
						send(connection->socket,error,error_size,0);
				}
			}
			break;

		case POP3C_CAPA:
			printf("hello\n");
			send(connection->socket,capa_sasl_list,capa_sasl_size,0);
			break;

		default:
			send(connection->socket,error,error_size,0);
	}
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  POP3_HandleConnectionData
 *  Description:  This function will handle data that is to be received from the 
 *                remote end.
 *-------------------------------------------------------------------------------------*/
void POP3_HandleConnectionData (POP3_CONNECTION *connection)
{
	switch(connection->command)
	{
		case POP3C_AUTH:
			if (connection->auth_type == POP3A_PLAIN)
			{
				/* we are expecting the user name - BASE64 encoded */
				/* TODO: actually check this. */
				send(connection->socket,ok,ok_size,0);
				connection->mode = POP3_TRANSACTION_MODE;
				connection->waiting_for_data = 0;

			}
			else if (connection->auth_type == POP3A_CRAM_MD5)
			{
				/* we are expecting the user name - BASE64 encoded */
				/* TODO: actually check this. */
				DumpHexMem(connection->buffer,connection->data_size);
				send(connection->socket,ok,ok_size,0);
				connection->mode = POP3_TRANSACTION_MODE;
				connection->waiting_for_data = 0;

			}
			else
			{
				send(connection->socket,error,error_size,0);
			}
			break;

		default:
			send(connection->socket,error,error_size,0);
	}
}

/*---  FUNCTION  ----------------------------------------------------------------------*
 *         Name:  main
 *  Description:  
 *-------------------------------------------------------------------------------------*/
void	main()
{
	int			err;
	int			closed;
	int			address_size;
	int			bytes_read;
	WORD		wVersionRequested;	
	WSADATA 	wsaData;
	SOCKET		s_listen,s_in;
	SOCKADDR_IN	sin_in,sin_listen,sin_addr;
	POP3_COMMANDS	command;
	POP3_CONNECTION	connection;

	wVersionRequested = MAKEWORD( 2, 2 ); 
	err = WSAStartup( wVersionRequested, &wsaData );
	
	if ( err != 0 ) 
	{	
		printf("Unable to find the WinSock DLL\n");
		exit(-1);
	}

	/* now lets talk to the sockets */
	s_listen = socket(AF_INET,SOCK_STREAM,0);

	if (s_listen == INVALID_SOCKET)
	{
		printf("Socket failed to open.\n");
	}else{
		/* now create a connection to the remote system
		 * we will connect to the address (to the local IP address: port 712)
		 */

		sin_listen.sin_family = AF_INET;
		sin_listen.sin_port   = htons(POP3_PORT);
		sin_listen.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(s_listen,(LPSOCKADDR)&sin_listen,sizeof(sin_listen)) == SOCKET_ERROR)
		{
			printf("Failed to bind to local socket. %d \n",WSAGetLastError());
		}else{
			if (listen(s_listen,1) == SOCKET_ERROR)
			{
				printf("failed to make the socket a listener \n");
			}
			else
			{
				s_in = accept(s_listen,NULL,NULL);

				if (s_in == INVALID_SOCKET)
				{
					printf("failed to open accept socket %d \n",WSAGetLastError());
				}else{
					closed = 0;

					connection.socket = s_in;
					connection.waiting_for_data = 0;

					/* sen the greeting */
					POP3_sendGreeting(&connection);

					while(command != POP3C_QUIT && (connection.data_size = recv(connection.socket,connection.buffer,POP3_MAX_BUFFER_SIZE,0)) > 0)
					{
						if (connection.waiting_for_data)
						{
							POP3_HandleConnectionData(&connection);
						}
						else if ((command = POP3_DecodeCommand(&connection)) != POP3C_QUIT)
						{
							printf("command: %d\n",command);
							POP3_HandleConnection(&connection);
						}
						printf("waiting for data: %d\n",connection.waiting_for_data);
					}

					if (connection.mode == POP3_UPDATE_MODE)
					{
						/* TODO: this needs to delete the messages that are marked for deletion */
						printf("deleting stuff - if you asked me too\n");
					}
			
					closesocket(s_in);
				}
			}
		}
		close(s_listen);
	}

	WSACleanup();
}

/* $Id$ */
