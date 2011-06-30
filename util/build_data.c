/*-------------------------------------------------------------------------------------*
 *
 * name:  build_data.c
 * proj:  Miniweb browser version 3
 *
 * desc:  This function will build a .c file with the data from a list of files
 *        that are passed in. 
 *
 * auth:  Peter Antoine  
 * date:  03/11/10
 *
 *               Copyright (c) 2009 Miniweb Interactive.
 *                       All rights Reserved.
 *-------------------------------------------------------------------------------------*/

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define	BUFFER_SIZE	(8 * 1204)

static unsigned char	HexBit[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

int	dump_file(int outfile, unsigned int item_number, char* file_name)
{
	int				infile;
	unsigned int	filesize = 0;
	unsigned int	pos,count,index;
	unsigned int	bytes_read;
	unsigned char	line[164];
	unsigned char	buffer[BUFFER_SIZE];
	unsigned char	preamble[] = "static unsigned char item_00000000[] = {\n";
	unsigned char	postamble[] = "};\n";
	
	if ((infile = open(file_name,O_RDONLY)) != -1)
	{
		pos = 33;
		while (item_number > 0)
		{
			preamble[pos--] = HexBit[item_number & 0x0f];
			item_number = item_number >> 4;
		}

		write(outfile,preamble,sizeof(preamble)-1);

		while ((bytes_read = read(infile,buffer,BUFFER_SIZE)) > 0)
		{
			index = 0;
			filesize += bytes_read;

			while ((index + 32) < bytes_read)
			{
				line[0] = '\t';

				for (pos=1,count=0;count<32;count++,pos+=5,index++)
				{
					line[pos  ] = '0';
					line[pos+1] = 'x';
					line[pos+2] = HexBit[(buffer[index] & 0xf0) >> 4];
					line[pos+3] = HexBit[(buffer[index] & 0x0f)];
					line[pos+4] = ',';
				}
				line[pos++] = 0x0a;

				write(outfile,line,pos);
			}

			if (index < bytes_read)
			{
				line[0] = '\t';

				for (pos=1; index < bytes_read; pos+=5,index++)
				{
					line[pos  ] = '0';
					line[pos+1] = 'x';
					line[pos+2] = HexBit[(buffer[index] & 0xf0) >> 4];
					line[pos+3] = HexBit[(buffer[index] & 0x0f)];
					line[pos+4] = ',';
				}

				if (index == bytes_read)
					line[pos] = ' ';

				line[pos++] = 0x0a;

				write(outfile,line,pos);
			}
		}

		write(outfile,postamble,sizeof(postamble)-1);

		close(infile);
	}

	return filesize;
}

int	main(int argc, char* argv[])
{
	int pos;
	int temp;
	int count;
	int index;
	int result = 1;
	int outfile;
	unsigned int	file_sizes[1000];
	unsigned char	name[] = "item_00000000,\x0a\x09";
	unsigned char	size[] = "0x00000000,\x0a\x09";
	unsigned char	data_preamble[] = "\x0aunsigned char* data_table[] = \n{\n\x09";
	unsigned char	data_postamble[] = ";\n";
	unsigned char	size_preamble[] = "\x0aunsigned int size_table[] = \n{\n\x09";
	unsigned char	size_postamble[] = ";\n";
	unsigned char	buffer[100];

	if (argc < 3)
	{
		printf("Error not enough parameters\n");
		printf("Usage: <output_filename> <input_file> [<input_file> [| ... ] ] \n");
	}
	else if ((outfile = open(argv[1],(O_CREAT|O_TRUNC|O_WRONLY),(S_IRUSR|S_IWUSR))) < 0)
	{
		perror("Failed to open file %s");
		printf("Usage: <output_filename> <input_file> [<input_file> [| ... ] ] \n");
	}
	else
	{
		for (count=2; count < argc && count < 1000; count++)
		{
			file_sizes[count-2] = dump_file(outfile,count-2,argv[count]);
		}

		if (count > 1000)
		{
			printf("FAILED: too many files \n");
		}
		else
		{
			/* write the data array */
			write(outfile,data_preamble,sizeof(data_preamble)-1);

			for (index=0;index<count-2;index++)
			{
				pos = 12;
				temp = index;
				while (temp > 0)
				{
					name[pos--] = HexBit[temp & 0x0f];
					temp = temp >> 4;
				}

				if (index+1 == count-2)
				{
					name[13] = ' ';
					name[15] = '}';
				}

				write(outfile,name,sizeof(name)-1);
			}
			write(outfile,data_postamble,sizeof(data_postamble)-1);

			/* write the size array */
			write(outfile,size_preamble,sizeof(size_preamble)-1);

			for (index=0;index<count-2;index++)
			{
				pos = 9;
				memset(&size[2],'0',8);
				temp = file_sizes[index];
				while (temp > 0)
				{
					size[pos--] = HexBit[temp & 0x0f];
					temp = temp >> 4;
				}

				if (index+1 == count-2)
				{
					size[10] = ' ';
					size[12] = '}';
				}

				write(outfile,size,sizeof(size)-1);
			}
			write(outfile,size_postamble,sizeof(size_postamble)-1);

			pos = sprintf(buffer,"\nunsigned int num_items = %d;\n\n",count-2);
			write(outfile,buffer,pos);
		}
	}

	close(outfile);

	return result;
}


/* $Id$ */

