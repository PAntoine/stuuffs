unsigned char	data_buffer[] = " {\n     \"firstName\": \"John\",\n     \"lastName\": \"Smith\",\n     \"age\": 25,\n     \"address\": {\n"
								"         \"streetAddress\": \"21 2nd Street\",\n         \"city\": \"New York\",\n         \"state\": \"NY\""
								",\n         \"postalCode\": \"10021\"\n     },\n     \"phoneNumber\": [\n         { \"type\": \"home\", \"num"
								"ber\": \"212 555-1234\" },\n         { \"type\": \"fax\", \"number\": \"646 555-4567\" }\n     ],\n     \"new"
								"Subscription\": false,\n     \"companyName\": null\n }\n\n";

unsigned int data_size = sizeof(data_buffer);


unsigned char   allowed_chars[] =
{
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,
        0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

typedef enum
{
	JSON_WAITING_FOR_START,
	JSON_WAITING_FOR_ELEMENT,
	JSON_FINISHED,
	JSON_NAME_STARTED,
	JSON_WAITING_FOR_SEP,
	JSON_WAITING_FOR_VALUE,
	JSON_VALUE_STARTED,
	JSON_WAITING_FOR_NEXT,
	JSON_FAILED

} JSON_DECODE_STATES;

int	JSON_GetString(unsigned char* in_buffer,unsigned int buffer_size, unsigned char* out_buffer, unsigned int out_size)
{
	unsigned int count = 0;
	unsigned int offset = 0;
	unsigned int string_start = 0;

	while(count < buffer_size)
	{
		if (in_buffer[count++] == '\"')
		{
			string_start = 1;
			break;
		}
	}

	if (string_start)
	{
		while (count < buffer_size && offset < out_size)
		{
			/* TODO: handle escape chars */
			if (in_buffer[count] != '\"')
			{
				out_buffer[offset] = in_buffer[count];
				offset++;
			}
			else
			{
				out_buffer[offset] = '\0';
				offset++;
				break;
			}
			count++;
		}
	}

	if (in_buffer[count] != '\"')
		return 0;
	else
		return offset;
}

int	JSON_DecodeNumber(unsigned char* buffer,unsigned int size, long* integer, double* floating_point)
{
	long			int_part = 0;
	long			float_part = 0;
	double			int_float;
	unsigned int	count = 0;
	unsigned int	int_digits = 0;
	unsigned int	float_digits = 0;
	unsigned char	failed = 0;
	unsigned char	is_float = 0;
	char	is_negative = 1;

	*integer = 0;
	*floating_point = 0;

	while(allowed_chars[buffer[count]] && count < size)
	{
		if (buffer[count] == '-')
		{
			if (int_digits > 0)
			{
				failed = 1;
				break;
			}
			else
			{
				is_negative = -1;
			}
		}
		else if (buffer[count] == '.')
		{
			if (is_float)
			{
				failed = 1;
				break;
			}
			else
			{
				is_float = 1; 
			}
		}
		else if (is_float)
		{
			float_digits++;
			float_part = ((float_part * 10) + (buffer[count] - '0'));
		}
		else
		{
			int_part = ((int_part * 10) + (buffer[count]- '0'));
		}

		count++;
	}

	if (buffer[count] != ' ' && int_part == 0 && float_part == 0)
	{
		failed = 1;
	}
	else
	{
		/* TODO: use a look-up table to remove the call to pow() */
		*integer = int_part * is_negative;
		*floating_point = (int_part + (float_part / pow(10.0,float_digits))) * is_negative;
	}

	if (failed)
		return 0;
	else
		return count;
}

unsigned int JSON_CheckLiteral(unsigned char* buffer)
{
	unsigned int result = 0;

	if (buffer[0] == 'f' || buffer[0] == 't' || buffer[0] == 'n')
	{
		if (memcmp(buffer,"false",5) == 0)
		{
			result += 4;
		}
		else if (memcmp(buffer,"true",4) == 0)
		{
			result += 3;
		}
		else if (memcmp(buffer,"null",4) == 0)
		{
			result += 3;
		}
	}

	return result;
}

void	JSON_parse_object(unsigned char* buffer, unsigned int size)
{
	long	number;
	double	b_number;
	unsigned int	count = 0;
	unsigned int	offset = 0;
	unsigned int	lit_size;
	unsigned int	object_level = 0;
	unsigned int	found_start = 0;
	unsigned int	name_count = 0;
	unsigned char	name[255];
	unsigned char	value[255];
	JSON_DECODE_STATES	state = JSON_WAITING_FOR_START;

	while(count < size && state != JSON_FAILED)
	{
		switch(state)
		{
			case JSON_WAITING_FOR_START:
					if (buffer[count] == '{')
						state = JSON_WAITING_FOR_ELEMENT;
					else if (buffer[count] == '}')
						state = JSON_FINISHED;
					break;

			case JSON_WAITING_FOR_ELEMENT:
					if (buffer[count] == '\"')
					{
						name_count = JSON_GetString(&buffer[count],size-count,name,255);
						printf("NAME: %s (%c)\n",name,buffer[count+name_count+1]);
						count += name_count;
						state = JSON_WAITING_FOR_SEP;
					}
					else if (buffer[count] == '{')
					{
						object_level++;
						state = JSON_WAITING_FOR_ELEMENT;
					}
					else if (!isspace(buffer[count]))
						state = JSON_FAILED;
					break;

			case JSON_WAITING_FOR_SEP:
					if (buffer[count] == ':')
						state = JSON_WAITING_FOR_VALUE;
					else if (!isspace(buffer[count]))
						state = JSON_FAILED;
					break;

			case JSON_WAITING_FOR_VALUE:
					if (buffer[count] == '\"')
					{
						count += JSON_GetString(&buffer[count],size-count,value,255);
						printf("string value: %s\n",value);
						state = JSON_WAITING_FOR_NEXT;
					}
					else if (buffer[count] == '{')
					{
						object_level++;
						state = JSON_WAITING_FOR_ELEMENT;
					}
					else if (buffer[count] == '[')
					{
						object_level++;
						state = JSON_WAITING_FOR_VALUE;
					}
					else if ((offset = JSON_DecodeNumber(&buffer[count],size-count,&number,&b_number)))
					{
						count += offset - 1;
						state = JSON_WAITING_FOR_NEXT;
					}
					else if ((offset = JSON_CheckLiteral(&buffer[count])))
					{
						count += offset;
						state = JSON_WAITING_FOR_NEXT;
					}
					else if (!isspace(buffer[count]))
					{
						state = JSON_FAILED;
					}
					break;

			case JSON_WAITING_FOR_NEXT:
					if (buffer[count] == ',')
					{
						state = JSON_WAITING_FOR_ELEMENT;
					}
					else if (buffer[count] == '}')
					{
						object_level--;
						JSON_WAITING_FOR_NEXT;
					}
					else if (buffer[count] == ']')
					{
						JSON_WAITING_FOR_NEXT;
					}
					else if (!isspace(buffer[count]))
					{
						state = JSON_FAILED;
					}
					break;
		}

		count++;
	}
}

unsigned char*	test_number[] = {"01234567890","-1","-1024","0.0","-1.0","-12222.50","-1011010.999999999","-9543535345.0004"};

int main()
{	
	int count,result;
	long	number;
	double	b_number;

	printf("test integer conversion\n");

	for (count=0;count<(sizeof(test_number)/sizeof(test_number[0]));count++)
	{
		result = JSON_DecodeNumber(test_number[count],strlen(test_number[count]),&number,&b_number);

		printf("[%d] result: %d integer: %ld double: %f\n",count,result,number,b_number);
	}

	printf("--------- Testing the actual parse --------\n");
	JSON_parse_object(data_buffer,data_size);
}

/* $Id$ */

