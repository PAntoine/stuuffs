#ifndef  __POP3_H__
#define  __POP3_H__
#include <winsock2.h>

#define	POP3_AUTHENTICATION_MODE	(0)
#define	POP3_TRANSACTION_MODE		(1)
#define	POP3_UPDATE_MODE			(2)

#define POP3_MAX_BUFFER_SIZE		(8*1024)

typedef enum
{
	POP3C_TOP,		/* WHY did they not just use a 4 char command!!!! */
	POP3C_APOP,
	POP3C_DELE,
	POP3C_LIST,
	POP3C_NOOP,
	POP3C_PASS,
	POP3C_QUIT,
	POP3C_RETR,
	POP3C_RSET,
	POP3C_STAT,
	POP3C_UIDL,
	POP3C_USER,
	POP3C_CAPA,
	POP3C_AUTH,
	POP3C_MAX_COMMANDS,
	POP3C_INVALID_COMMAND = POP3C_MAX_COMMANDS

} POP3_COMMANDS;

typedef enum
{
	POP3A_PLAIN,
	POP3A_CRAM_MD5

} POP3_AUTH_TYPES;



typedef struct
{
	char*			name;
	unsigned int	length;
	unsigned int	value;
} POP3_NAMES;


typedef struct
{
	SOCKET			socket;
	POP3_COMMANDS	command;
	POP3_AUTH_TYPES	auth_type;
	unsigned int	mode;
	unsigned int	waiting_for_data;
	unsigned int	data_size;
	char			buffer[POP3_MAX_BUFFER_SIZE];

} POP3_CONNECTION;



#endif
