/*-------------------------------------------------------------------------------------*
 *
 * name:  test_md5.cpp
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  12/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

typedef struct
{
		unsigned char*	key;
		unsigned char*	data;
		unsigned int	length;
} HMAC_Pairs;

char*	test_array[] = {	"",
							"a",
							"abc",
							"message digest",
							"abcdefghijklmnopqrstuvwxyz",
							"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
							"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
							"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"};

unsigned char ob_key[] = {0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b,0x0b};
unsigned char aa_key[] = {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
unsigned char dd_data[] = { 0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
							0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
							0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
							0xdd,0xdd,0x0};

HMAC_Pairs	test_data[] = {	{ob_key,"Hi There",sizeof(ob_key)},
							{"Jefe","what do ya want for nothing?",sizeof("Jefe")-1},
							{aa_key,dd_data,sizeof(aa_key)}};

void 			tls_MD5Setup ( void );
void			tls_MD5Init ( unsigned int* md5_digest );
unsigned int	tls_MD5Hash ( unsigned char* input_buffer, unsigned int input_size_bits, unsigned int* md5_digest );
void			tls_MD5Finish ( unsigned char* input_buffer, unsigned int input_size_bits, unsigned int* md5_digest, unsigned int total_bits, unsigned char* md5_hash );

void	MD5HMACHash(unsigned char* kpad,unsigned char *text,unsigned int text_length,unsigned char* digest)
{
	unsigned int	processed;
	unsigned int	md5_digest[4];

	int count;

	tls_MD5Init(md5_digest);

	tls_MD5Hash(kpad,64*8,md5_digest);
	processed = tls_MD5Hash(text,text_length*8,md5_digest);

	tls_MD5Finish((unsigned char*)&text[processed],((text_length - processed) * 8),md5_digest,((text_length + 64)*8),digest);
}

void	HMAC_function(unsigned char* text, unsigned int text_length,unsigned char* key, unsigned int key_length)
{
	unsigned int	count;
	unsigned int	B = 64; 	/* this may change with the hash function */
	unsigned char	k_opad[64];
	unsigned char	k_ipad[64];
	unsigned char	digest[16];
	unsigned char	result[16];

	if (key_length > 64)
	{
		/* hash the key */
		/* dont handle this at the moment */
	}
	else
	{
		/* make the munged keys */
		for (count=0;count<key_length;count++)
		{
			k_opad[count] = key[count] ^ 0x5c;
			k_ipad[count] = key[count] ^ 0x36;
		}
		for (;count<64;count++)
		{
			k_opad[count] = 0x5c;
			k_ipad[count] = 0x36;
		}
	}

	/* now do the Hashes */
//	switch(hash_function)
//	{
//		case MD5:	
					MD5HMACHash(k_ipad,text,text_length,digest);
					MD5HMACHash(k_opad,digest,16,result);
//					break;
//		default:
//					/* only handles MD5 for now */
//					/* TODO: you know you need to */
//	}

	/* generate the result as a TLS vector */
	printf("MD5: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",
			result[0],result[1],result[2],result[3],
			result[4],result[5],result[6],result[7],
			result[8],result[9],result[10],result[11],
			result[12],result[13],result[14],result[15]);
}

void main()
{
	int				count;
	unsigned char	md5_hash[16];
	unsigned int 	data[256];
	unsigned int	md5_digest[4];
	unsigned int	processed;
	unsigned int	bit_length;

	tls_MD5Setup();

	for (count = 0;count<8;count++)
	{
		tls_MD5Init(md5_digest);
		bit_length = strlen(test_array[count])*8;
		processed = tls_MD5Hash((unsigned char*)test_array[count],bit_length,md5_digest);
		tls_MD5Finish((unsigned char*)&test_array[count][processed],bit_length - (processed * 8),md5_digest,bit_length,md5_hash);

		printf("MD5: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",
				md5_hash[0],md5_hash[1],md5_hash[2],md5_hash[3],
				md5_hash[4],md5_hash[5],md5_hash[6],md5_hash[7],
				md5_hash[8],md5_hash[9],md5_hash[10],md5_hash[11],
				md5_hash[12],md5_hash[13],md5_hash[14],md5_hash[15]);
	}

	/* dual call test */
	tls_MD5Init(md5_digest);
	bit_length = strlen(test_array[7])*8;
	processed = tls_MD5Hash((unsigned char*)test_array[7],82*8,md5_digest);
	processed += tls_MD5Hash((unsigned char*)&test_array[7][processed],bit_length-(82*8),md5_digest);
	tls_MD5Finish((unsigned char*)&test_array[7][processed],bit_length - (processed * 8),md5_digest,bit_length,md5_hash);

	printf("should match the entry above\n");
	printf("MD5: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",
			md5_hash[0],md5_hash[1],md5_hash[2],md5_hash[3],
			md5_hash[4],md5_hash[5],md5_hash[6],md5_hash[7],
			md5_hash[8],md5_hash[9],md5_hash[10],md5_hash[11],
			md5_hash[12],md5_hash[13],md5_hash[14],md5_hash[15]);


	/* now lets test if it can be used for a HMAC */
	printf("\nNow test the HMAC functions\n");
	for (count=0;count<3;count++)
	{
		HMAC_function(test_data[count].data,strlen(test_data[count].data),test_data[count].key,test_data[count].length);
	}
}


/* $Id$ */

