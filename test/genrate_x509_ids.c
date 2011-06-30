/*-------------------------------------------------------------------------------------*
 *
 * name:  genrate_x509_ids.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  19/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/


typedef struct
{
	unsigned char*	name;
	unsigned int	iso;
	unsigned int	member_body;
	unsigned int	fields;
	unsigned int	field[10];
} SIGNATURES;

SIGNATURES	sigs[] = {
					{"PKIX1Algorithms88", 1,3,7,{ 6, 1, 5, 5, 7, 0, 17 }},
					{"ansi-X9-62",1,2,2,{ 840, 10045 }},
					{"dhpublicnumber",1,2,4,{ 840, 10046, 2, 1 }},
					{"id-dsa",1,2,4,{ 840, 10040, 4, 1 }},
					{"id-dsa-with-sha1", 1,2,4,{ 840, 10040, 4, 3 }},
					{"id-sha1",1,3,4,{ 14, 3, 2, 26 }},
					{"md2",1,2,4,{ 840, 113549, 2, 2 }},
					{"md2WithRSAEncryption", 1,2,5,{ 840, 113549, 1, 1, 2  }},
					{"md5",1,2,4,{ 840, 113549, 2, 5 }},
					{"md5WithRSAEncryption", 1,2,5,{ 840, 113549, 1, 1, 4  }},
					{"pkcs-1",1,2,4,{ 840, 113549, 1, 1 } },
					{"sha-1WithRSAEncryption", 1,2,5,{ 840, 113549, 1, 1, 5  }},
					{"id-domainComponent",0,9,5,{2342,19200300,100,1,25}},
					{"holdInstruction",2,2,3,{840,10040,2}},
					{"kea",1,16,6,{840,1,101,2,1,1,22}},
					{"netscap_comment",2,16,4,{840,1,113730,13}},
					{"netscap_certype",2,16,4,{840,1,113730,1}},
					{"id-ce",2,5,1,{29}},
				};

#define NUMBER_OF(x)	((sizeof(x)/sizeof(x[0])))


main()
{
	unsigned int	count,kount,qount;
	unsigned long	value;
	unsigned char	body;
	unsigned char	byte[10];

	for (count=0;count<NUMBER_OF(sigs);count++)
	{
		body = sigs[count].iso * 40 + sigs[count].member_body;

		printf("name: %s %02x,",sigs[count].name,body);

		for (kount=0;kount<sigs[count].fields;kount++)
		{
			value = sigs[count].field[kount];

			for (qount=0;qount < 10 && value > 0;qount++)	
			{
				byte[qount] = value & 0x7F;
				value >>= 7;
			}

			if (qount == 0)
				printf("its zero\n");
			else
				do
				{
					qount--;
					if (qount == 0)
						printf("%02x,",byte[qount]);
					else
						printf("%02x,",byte[qount] | 0x80);

				}
				while(qount > 0);
		}

		printf("\n");
		
	}
}

/* $Id$ */

