/***************************************************************************************
 *
 *       Filename:  ASN.1 Decoder
 *
 *    Description:  This file holds the structures required for the ASN.1 decoder.
 *
 *        Version:  1.0
 *        Created:  20/03/2009 10:12:03
 *       Revision:  none
 *
 *         Author:  Peter Antoine
 *          Email:  me@peterantoine.me.uk
 *
 *------------------------------------------------------------------------------------- 
 *                         Copyright (c) 2009 : Peter Antoine
 *                        Released under the Artistic License.
 ***************************************************************************************/

/*-----------------------------------------------------------------------------*
 *  Basic Structures for the Decode.
 *-----------------------------------------------------------------------------*/
typedef	struct
{
	unsigned char	*asn1_data;
	unsigned int	data_size;
	unsigned int	current_offset;

} ASN1_STATUS;

/*-----------------------------------------------------------------------------*
 *  ASN1 enum types
 *-----------------------------------------------------------------------------*/
typedef enum
{
	A1C_UNIVERSAL			= 0x00,
	A1C_APPLICATION			= 0x01,
	A1C_CONTEXT_SPECIFIC	= 0x02,
	A1C_PRIVATE				= 0x03

} ASN1_CLASS;

typedef enum
{
	A1P_PRIMITIVE  	= 0,
	A1P_CONSTRUCTED	= 1

} ASN1_PC_TYPE;

typedef	enum
{
	A1UC_NOT_USED				= 0,
	A1UC_BOOLEAN_TYPE			= 1,
	A1UC_INTEGER_TYPE			= 2,
	A1UC_BITSTRING_TYPE			= 3,
	A1UC_OCTETSTRING_TYPE		= 4,
	A1UC_NULL_TYPE				= 5,
	A1UC_OBJECT_IDENTIFIER_TYPE	= 6,
	A1UC_OBJECT_DESCRIPTOR_TYPE	= 7,
	A1UC_EXTERNAL_TYPE			= 8,
	A1UC_REAL_TYPE				= 9,
	A1UC_ENUMERATED_TYPE		= 10,
	A1UC_EMBEDDED_PDV_TYPE		= 11,
	A1UC_UTF8STRING_TYPE		= 12,
	A1UC_RELATIVE_OBJECT_TYPE	= 13,
	A1UC_SEQUENCE_TYPE			= 16,
	A1UC_SET_TYPE				= 17,
	A1UC_NUMERICSTRING_TYPE		= 18,
	A1UC_PRINTABLESTRING_TYPE	= 19,
	A1UC_TELETEXSTRING_TYPE 	= 20,
	A1UC_VIDEOTEXSTRING_TYPE	= 21,
	A1UC_IA5STRING_TYPE			= 22,
	A1UC_UTC_TIME_TYPE			= 23,
	A1UC_GENERALIZED_TIME_TYPE	= 24,
	A1UC_GRAPHICSTRING_TYPE		= 25,
	A1UC_VISIBLESTRING_TYPE 	= 26,
	A1UC_GENERALSTRING_TYPE		= 27,
	A1UC_UNIVERSALSTRING_TYPE	= 28,
	A1UC_BMPSTRING_TYPE			= 30

} ASN1_UNIVERSAL_CLASS;

/*-----------------------------------------------------------------------------*
 *  ASN1 Decode Functions.
 *-----------------------------------------------------------------------------*/
void ASN1_initialise (ASN1_STATUS *asn1, unsigned char* buffer, unsigned int buffer_size);
unsigned int	ASN1_GetNextElement (ASN1_STATUS* asn1, unsigned long* ID_tag,ASN1_CLASS *class, ASN1_PC_TYPE *type, unsigned long* length);

