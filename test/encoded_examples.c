static const char *RcsId = "$Id$\n$Name$";
/******************************************************************************
 * 
 * File       :	encoded_examples.c
 *
 * Project    :	C client interface to Tango
 * 
 * Description:	  Examples how to handle attribute of the DevEncoded type 
 *                with the Tango C API
 *                Needs a device with an attribute of this type.
 *
 * Original   :	November 2010	
 *	
 * $Author$
 *
 * $Log$
 *
 ******************************************************************************/ 


#include <stdio.h>
#include <unistd.h>

#include <c_tango.h>


main (unsigned int argc, char **argv)
{
	void 	   *proxy;
	ErrorStack error;


	if (argc < 2)
	{
	   printf("usage: %s device_name enoded_attribute_name\n",argv[0]);
	   exit(1);
	}

	if ( !tango_create_device_proxy (argv[1], &proxy, &error) )
	{
		tango_print_ErrorStack (&error);
		tango_free_ErrorStack (&error);

		exit(1);
	}
	
		
	AttributeData enc_data;
		
	if (!tango_read_attribute (proxy, argv[2], &enc_data, &error))
	{
		tango_print_ErrorStack (&error);
		tango_free_ErrorStack (&error);
	}
	else
	{
		if ( enc_data.quality == ATTR_INVALID )
		{
			printf ("Data quality is INVALID!\n");
		}
		else
		{
			if ( enc_data.data_type != DEV_ENCODED )
			{
				printf ("The data type is not DEV_ENCODED!\n");
			}
			else
			{
				int j;
				
				for (j=0; j<enc_data.dim_x; j++)
				{
					printf ("Attribute read value:\n");
					printf ("Format = %s\n", enc_data.attr_data.encoded_arr.sequence[j].encoded_format);
					printf ("Length = %d\n", enc_data.attr_data.encoded_arr.sequence[j].encoded_length);
					printf ("Data   = %d, %d, %d\n", enc_data.attr_data.encoded_arr.sequence[j].encoded_data[0],
					                                 enc_data.attr_data.encoded_arr.sequence[j].encoded_data[1],
													 enc_data.attr_data.encoded_arr.sequence[j].encoded_data[2]);
				}
				for (j=enc_data.dim_x; j<enc_data.attr_data.encoded_arr.length; j++)
				{
					printf ("Attribute set value:\n");
					printf ("Format = %s\n", enc_data.attr_data.encoded_arr.sequence[j].encoded_format);
					printf ("Length = %d\n", enc_data.attr_data.encoded_arr.sequence[j].encoded_length);
					printf ("Data   = %d, %d, %d\n", enc_data.attr_data.encoded_arr.sequence[j].encoded_data[0],
					                                 enc_data.attr_data.encoded_arr.sequence[j].encoded_data[1],
													 enc_data.attr_data.encoded_arr.sequence[j].encoded_data[2]);
				}				
			}
		}
	}
	
	tango_free_AttributeData (&enc_data);
			
}
		
