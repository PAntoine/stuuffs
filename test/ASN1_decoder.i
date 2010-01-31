#line 1 "ASN1_decoder.c"





























#line 1 "g:\\projects\\work_in_progress\\test\\ASN1_decoder.h"





















typedef	struct
{
	unsigned char	*asn1_data;
	unsigned int	data_size;
	unsigned int	current_offset;
	unsigned int	element_size;
	unsigned char*	element_data;

} ASN1_STATUS;




typedef enum
{
	A1C_UNIVERSAL			= 0x00,
	A1C_APPLICATION			= 0x01,
	A1C_CONTEXT_SPECIFIC	= 0x02,
	A1C_PRIVATE				= 0x03

} ASN1_CLASS;

typedef enum
{
	A1T_PRIMITIVE  	= 0,
	A1T_CONSTRUCTED	= 1

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




typedef struct
{
	ASN1_PC_TYPE	type;
	ASN1_CLASS		asn1_class;
	unsigned long	tag;
	unsigned long	length;
	unsigned char	*data;
	unsigned long	offset;				

} ASN1_OBJECT;











unsigned int	ASN1_GetObject (unsigned char* source_data,ASN1_OBJECT* object);
unsigned int	ASN1_GetNextObject (ASN1_OBJECT* source_object, ASN1_OBJECT* sub_object);

#line 31 "ASN1_decoder.c"





void ASN1_initialise (ASN1_STATUS *asn1, unsigned char* buffer, unsigned int buffer_size)
{
	asn1->asn1_data = buffer;
	asn1->data_size = buffer_size;
	asn1->current_offset = 0;
	asn1->element_size = 0;
	asn1->element_data = 0L;
}










static unsigned int	ASN1_DecodeHeader ( unsigned char* source_data, ASN1_OBJECT* object)
{
	unsigned int	end;
	unsigned int	worked = 1;
	unsigned int	offset = 0;
	unsigned int	tag_size;

	
	object->length	= 0;
	object->data	= 0L;
	object->offset	= 0;

	
	object->asn1_class	= ((source_data[0] & 0xc0) >> 6);
	object->type		= ((source_data[0] & 0x20) >> 5);

	if ((object->tag = (source_data[0] & 0x1F)) != 0x1f)
	{
		offset++;
	}
	else
	{
		



		object->tag = 0;

		do
		{
			offset++;
			object->tag = ((object->tag << 7) | (source_data[offset] & 0x7f));
		}
		while ((source_data[offset] & 0x80) != 0 && (object->tag & 0x0E000000) == 0);

		if ((source_data[offset] & 0x80) != 0)
		{
			
			worked = 0;
		}
		offset++;
	}

	
	if (worked)
	{
		if (source_data[offset] == 0)
		{
			






			offset++;
			object->length = 0;
		}
		else if ((source_data[offset] & 0x80) == 0)
		{
			
			object->length = source_data[offset];
			offset++;
		}
		else if ((source_data[offset] & 0x80) != 0)
		{
			
			end = offset + (source_data[offset] & 0x7f) +1;
			offset++;
			object->length = 0;

			for (;offset < end;offset++)
			{
				object->length = (object->length << 8) | source_data[offset];
			}
		}

		
		object->data = &source_data[offset];
	}

	if (worked == 0)
		return 0;
	else
		return offset;
}







unsigned int ASN1_GetObject ( unsigned char* source_data,ASN1_OBJECT* object )
{
	return ASN1_DecodeHeader(source_data,object);
}






unsigned int ASN1_GetNextObject ( ASN1_OBJECT* source_object, ASN1_OBJECT* sub_object )
{
	unsigned int	header_size = 0;
	unsigned char*	data;

	if (source_object->length > source_object->offset)
	{
		
		data = &source_object->data[source_object->offset];

		if ((header_size = ASN1_DecodeHeader(data,sub_object)))
		{
			
			source_object->offset += sub_object->length + header_size;
		}
	}

	return header_size;
}

