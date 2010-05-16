/*-------------------------------------------------------------------------------------*
 *
 * name:  pop3_tables.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  14/05/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include "pop3.h"

char	*pop_command_strings[] =
{
	"TOP ",
	"APOP",
	"DELE",
	"LIST",
	"NOOP",
	"PASS",
	"QUIT",
	"RETR",
	"RSET",
	"STAT",
	"UIDL",
	"USER",
	"CAPA",
	"AUTH"
};

POP3_NAMES	auth_types[] = {
							{ "PLAIN",		sizeof("PLAIN")-1, 		POP3A_PLAIN},
							{ "CRAM-MD5",	sizeof("CRAM-MD5")-1,	POP3A_CRAM_MD5}
						};

unsigned int auth_size = (sizeof(auth_types)/sizeof(auth_types[0]));


