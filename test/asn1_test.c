/*-------------------------------------------------------------------------------------*
 *
 * name:  asn1_test.c
 * proj:  Miniweb browser version 3
 *
 * desc:  
 *
 * auth:  Peter Antoine  
 * date:  15/01/2010
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include "homeserver.h"
#include "ASN1_decoder.h"
#include "X509_encoding.h"

char*	class_str[] = {
						"UNIVERSAL",
						"APPLICATION",
						"CONTEXT_SPECIFIC",
						"PRIVATE"
};

char*	universal_str[] = {
						"NOT_USED",
						"BOOLEAN_TYPE",
						"INTEGER_TYPE",
						"BITSTRING_TYPE",
						"OCTETSTRING_TYPE",
						"NULL_TYPE",
						"OBJECT_IDENTIFIER_TYPE",
						"OBJECT_DESCRIPTOR_TYPE",
						"EXTERNAL_TYPE",
						"REAL_TYPE",
						"ENUMERATED_TYPE",
						"EMBEDDED_PDV_TYPE",
						"UTF8STRING_TYPE",
						"RELATIVE_OBJECT_TYPE",
						"NOT USED 14",
						"NOT USED 15",
						"SEQUENCE_TYPE",
						"SET_TYPE",
						"NUMERICSTRING_TYPE",
						"PRINTABLESTRING_TYPE",
						"TELETEXSTRING_TYPE ",
						"VIDEOTEXSTRING_TYPE",
						"IA5STRING_TYPE",
						"UTC_TIME_TYPE",
						"GENERALIZED_TIME_TYPE",
						"GRAPHICSTRING_TYPE",
						"VISIBLESTRING_TYPE ",
						"GENERALSTRING_TYPE",
						"UNIVERSALSTRING_TYPE",
						"BMPSTRING_TYPE"
};

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



void main()
{
	long			version;
	unsigned long	tag;
	unsigned long	length;
	ASN1_OBJECT		container;
	ASN1_OBJECT		sub_levels[5];
	ASN1_OBJECT		tbsCert;
	ASN1_OBJECT		signature_algo;
	ASN1_OBJECT		signature;
	ASN1_OBJECT		object;
	ASN1_CLASS 		class;
	ASN1_PC_TYPE 	type;
	X509_CERTIFICATE	certificate;

	unsigned int	a,x,y,z;

	if (X509_DecodeCertificate (&certificate,homeserver_cert_der))
	{
		printf("It worked\n");
		hexDump(&certificate,sizeof(X509_CERTIFICATE));
	}
	else
	{
		printf("It failed\n");
	}

/* certificate decode that works */

//	if ((a = ASN1_GetObject(homeserver_cert_der,&container)))
//	{
//		if (container.type == A1T_CONSTRUCTED && container.asn1_class == A1C_UNIVERSAL)
//		{
//			if (ASN1_GetNextObject(&container,&tbsCert))
//			{
//				if (container.type == A1T_CONSTRUCTED && container.asn1_class == A1C_UNIVERSAL)
//				{
//					/* we should have the certificate now */
//					if (ASN1_GetNextObject(&tbsCert,&object) && ASN1_CHECK_OBJECT(object,A1T_CONSTRUCTED,A1C_CONTEXT_SPECIFIC,0) )
//					{
//						/* this is a sub-object of CONSTRUCTED TYPE */
//						if (ASN1_GetNextObject(&object,&sub_levels[0]) && ASN1_CHECK_OBJECT(sub_levels[0],A1T_PRIMITIVE,A1C_UNIVERSAL,A1UC_INTEGER_TYPE) )
//						{
//							version = ASN1_DecodeInteger(&sub_levels[0]);
//							printf("version: %d\n",version);
//						}
//
//						/* the serial number */
//						if (ASN1_GetNextObject(&tbsCert,&object) && ASN1_CHECK_OBJECT(object,A1T_PRIMITIVE,A1C_UNIVERSAL,A1UC_INTEGER_TYPE) )
//						{
//							version = ASN1_DecodeInteger(&object);
//
//							printf("serial no.: %d\n",version);
//						}
//
//						/* the signature algorithm */
//						if (ASN1_GetNextObject(&tbsCert,&object) && ASN1_CHECK_OBJECT(object,A1T_CONSTRUCTED,A1C_UNIVERSAL,A1UC_SEQUENCE_TYPE) )
//						{
//							printf("found the signture algorithm \n");
//							if (ASN1_GetNextObject(&object,&sub_levels[0]) && ASN1_CHECK_OBJECT(sub_levels[0],A1T_PRIMITIVE,A1C_UNIVERSAL,A1UC_OBJECT_IDENTIFIER_TYPE) )
//							{
//								hexDump(sub_levels[0].data,sub_levels[0].length);
//							}
//
//							/* find any parameters - this should depend on the type */
//							while (ASN1_GetNextObject(&object,&sub_levels[0]) && !ASN1_CHECK_OBJECT(sub_levels[0],A1T_PRIMITIVE,A1C_UNIVERSAL,A1UC_NULL_TYPE))
//							{
//								printf("We have parameters: \n");
//								printf("cert: Type %c Length: %04d class %s uni: %s\n",sub_levels[0].type?'C':'P',sub_levels[0].length,class_str[sub_levels[0].asn1_class],universal_str[sub_levels[0].tag]);
//								hexDump(sub_levels[0].data,sub_levels[0].length);
//							}
//						}
//
//						/* decode the issuer name */
//						if (ASN1_GetNextObject(&tbsCert,&object) && ASN1_CHECK_OBJECT(object,A1T_CONSTRUCTED,A1C_UNIVERSAL,A1UC_SEQUENCE_TYPE) )
//						{
//							printf("have the issuers name \n");
//							while(ASN1_GetNextObject(&object,&sub_levels[0]))
//							{
//								printf("cert: Type %c Length: %04d class %s uni: %s\n",sub_levels[0].type?'C':'P',sub_levels[0].length,class_str[sub_levels[0].asn1_class],universal_str[sub_levels[0].tag]);
//								while(ASN1_GetNextObject(&sub_levels[0],&sub_levels[1]))
//								{
//									printf("cert: Type %c Length: %04d class %s uni: %s\n",sub_levels[1].type?'C':'P',sub_levels[1].length,class_str[sub_levels[1].asn1_class],universal_str[sub_levels[1].tag]);
//									while(ASN1_GetNextObject(&sub_levels[1],&sub_levels[2]))
//									{
//										printf("cert: Type %c Length: %04d class %s uni: %s\n",sub_levels[2].type?'C':'P',sub_levels[2].length,class_str[sub_levels[2].asn1_class],universal_str[sub_levels[2].tag]);
//									}
//								}
//							}
//						}
//
//						/* decode the issuer name */
//						if (ASN1_GetNextObject(&tbsCert,&object) && ASN1_CHECK_OBJECT(object,A1T_CONSTRUCTED,A1C_UNIVERSAL,A1UC_SEQUENCE_TYPE) )
//						{
//							printf("have the validity \n");
//						}
//
//						/* decode the issuer name */
//						if (ASN1_GetNextObject(&tbsCert,&object) && ASN1_CHECK_OBJECT(object,A1T_CONSTRUCTED,A1C_UNIVERSAL,A1UC_SEQUENCE_TYPE) )
//						{
//							printf("have the subject name \n");
//						}
//
//						/* have the public Key info */
//						if (ASN1_GetNextObject(&tbsCert,&object) && ASN1_CHECK_OBJECT(object,A1T_CONSTRUCTED,A1C_UNIVERSAL,A1UC_SEQUENCE_TYPE) )
//						{
//							printf("have the public key information \n");
//
//							if (ASN1_GetNextObject(&object,&sub_levels[0]) && ASN1_CHECK_OBJECT(sub_levels[0],A1T_CONSTRUCTED,A1C_UNIVERSAL,A1UC_SEQUENCE_TYPE) )
//							{
//								printf("found the algorithm within the public key\n");
////								printf("found the signture algorithm \n");
////								if (ASN1_GetNextObject(&object,&sub_levels[0]) && ASN1_CHECK_OBJECT(sub_levels[0],A1T_PRIMITIVE,A1C_UNIVERSAL,A1UC_OBJECT_IDENTIFIER_TYPE) )
////								{
////									hexDump(sub_levels[0].data,sub_levels[0].length);
////								}
////
////								/* find any parameters - this should depend on the type */
////								while (ASN1_GetNextObject(&object,&sub_levels[0]) && !ASN1_CHECK_OBJECT(sub_levels[0],A1T_PRIMITIVE,A1C_UNIVERSAL,A1UC_NULL_TYPE))
////								{
////									printf("We have parameters: \n");
////									printf("cert: Type %c Length: %04d class %s uni: %s\n",sub_levels[0].type?'C':'P',sub_levels[0].length,class_str[sub_levels[0].asn1_class],universal_str[sub_levels[0].tag]);
////									hexDump(sub_levels[0].data,sub_levels[0].length);
////								}
//							}
//
//							if (ASN1_GetNextObject(&object,&sub_levels[0]) && ASN1_CHECK_OBJECT(sub_levels[0],A1T_PRIMITIVE,A1C_UNIVERSAL,A1UC_BITSTRING_TYPE) )
//							{
//								printf("xx\n");
//								ASN1_GetObject(&sub_levels[0].data[1],&sub_levels[1]);
//								printf("cert: Type %c Length: %04d class %s uni: %s\n",sub_levels[1].type?'C':'P',sub_levels[1].length,class_str[sub_levels[1].asn1_class],universal_str[sub_levels[1].tag]);
//								ASN1_GetNextObject(&sub_levels[1],&sub_levels[2]);
//								printf("cert: Type %c Length: %04d class %s uni: %s\n",sub_levels[2].type?'C':'P',sub_levels[2].length,class_str[sub_levels[1].asn1_class],universal_str[sub_levels[2].tag]);
//								hexDump(sub_levels[2].data,sub_levels[2].length);
//								ASN1_GetNextObject(&sub_levels[1],&sub_levels[2]);
//								printf("cert: Type %c Length: %04d class %s uni: %s\n",sub_levels[2].type?'C':'P',sub_levels[2].length,class_str[sub_levels[1].asn1_class],universal_str[sub_levels[2].tag]);
//								hexDump(sub_levels[2].data,sub_levels[2].length);
//
//							}
//						}
//					}
//
//					printf("cert: Type %c Length: %04d class %s uni: %s\n",object.type?'C':'P',object.length,class_str[object.asn1_class],universal_str[object.tag]);
//				}
//			}
//
//			if((y = ASN1_GetNextObject(&container,&signature_algo)))
//			{
//				printf("cont: Type %c Length: %04d class %s uni: %s\n",sub_levels[0].type?'C':'P',sub_levels[0].length,class_str[sub_levels[0].asn1_class],universal_str[sub_levels[0].tag]);
//			}
//
//			{
//				unsigned char x[256];
//
//				z = ASN1_GetNextObject(&container,&signature);
//				hexDump(signature.data,signature.length);
//				printf("length: %d <-\n",ASN1_DecodeBitString(&signature,x,256));
//				hexDump(x,1024/8);
//			}
//
//			printf("length: %d %d\n",container.length,a);
//			printf("yes: x: %d y: %d z: %d\n",x,y,z);
//			printf("algo: Type %c Length: %04d class %s uni: %s\n",signature_algo.type?'C':'P',signature_algo.length,class_str[signature_algo.asn1_class],universal_str[signature_algo.tag]);
//			printf("sig : Type %c Length: %04d class %s uni: %s\n",signature.type?'C':'P',signature.length,class_str[signature.asn1_class],universal_str[signature.tag]);
//		}
//


/* old hack */

//	while (ASN1_GetNextElement(&status,&tag,&class,&type,&length))
//	{
//		printf("Element: Type %c Length: %04d class %s",type?'C':'P',length,class_str[class]);
//
//		if (class == A1C_UNIVERSAL)
//		{
//			printf(" %s\n",universal_str[tag]);
//
//			if (!type)
//			{
//				switch (tag)
//				{
//					case A1UC_OBJECT_IDENTIFIER_TYPE:
//					{
//						printf("OBJECT IDENTIFIER\n");
//
//						if (status.element_data[0] == X520_ID_SIG)
//						{
//							if (status.element_data[1] == X520_ID_AT)
//							{
//								printf("Attribute:\n");
//
//							}
//							else if (status.element_data[1] == X520_ID_CE)
//							{
//								printf("Element:\n");
//							}
//							else
//							{
//								printf("unknown object descriptor\n");
//							}
//						}
//					}
//					break;
//
//				}
//			}
//		}
//		else
//		{
//			printf(" %d %04x\n",tag,tag);
//		}
//
//		if (!type)
//		{
//			/* dump the primatives */
//			hexDump(status.element_data,status.element_size);
//		}
//	}
}

/* $Id$ */

