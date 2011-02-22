static const char *RcsId = "$Id$\n$Name$";
/******************************************************************************
 * 
 * File       :	write_examples.c
 *
 * Project    :	C client interface to Tango
 * 
 * Description:	Write examples with the Tango C API
 *                This examples can be executed on a device of the TangoTest class.
 *                The TangoTest class is part of the Tango distribution and available
 *                with every Tango installation.
 *
 * Original   :	November 2007	
 *	
 * $Author$
 *
 * $Log$
 * Revision 1.2  2007/12/07 16:06:43  jensmeyer
 * Some name changes to be comaptible with Taco
 *
 * Revision 1.1.1.1  2007/12/05 15:05:05  jensmeyer
 * Tango C language binding
 *
 *
 ******************************************************************************/ 


#include <stdio.h>
#include <unistd.h>

#include <c_tango.h>


main (unsigned int argc, char **argv)
{
	void 			*proxy;
	ErrorStack error;

	char			cmd_string [80];
	long 			cmd;
	
	double 				d_val = 11.22;
	TangoDevLong	 l_val = 1234;


	if (argc < 2)
		{
	   printf("usage: %s device_name\n",argv[0]);
	   exit(1);
		}

	if ( !tango_create_device_proxy (argv[1], &proxy, &error) )
		{
		tango_print_ErrorStack (&error);
		tango_free_ErrorStack (&error);

		exit(1);
		}
	
		/* write command with scalar value */
		
		{
		CommandData cmd_in, cmd_out;
			
		cmd_in.arg_type            = DEV_DOUBLE;
		cmd_in.cmd_data.double_val = d_val;
		
		if (!tango_command_inout (proxy, "DevDouble", &cmd_in, &cmd_out, &error))
			{
			tango_print_ErrorStack (&error);
			tango_free_ErrorStack (&error);
			}
		else
			{
			if ( cmd_out.arg_type == DEV_DOUBLE )
				{
				printf ("cmd result = %f\n\n", cmd_out.cmd_data.double_val);
				}
			
			tango_free_CommandData (&cmd_out);
			}
		}

		/* write command with array value */
		
		{
		CommandData cmd_in, cmd_out;
		double arr[2];
		int i;
		
		arr[0] = d_val;
		arr[1] = d_val + 1.11;
		
		cmd_in.arg_type = DEVVAR_DOUBLEARRAY;
		cmd_in.cmd_data.double_arr.length =   2;
		cmd_in.cmd_data.double_arr.sequence = &(arr[0]);
		
		if (!tango_command_inout (proxy, "DevVarDoubleArray", &cmd_in, &cmd_out, &error))
			{
			tango_print_ErrorStack (&error);
			tango_free_ErrorStack (&error);
			}
		else
			{
			if ( cmd_out.arg_type == DEVVAR_DOUBLEARRAY )
				{
				for (i=0; i<cmd_out.cmd_data.long_arr.length; i++)
					{
					printf ("[%d] cmd result = %f\n", i, cmd_out.cmd_data.double_arr.sequence[i]);
					}				
				printf ("\n");
				}
			
			tango_free_CommandData (&cmd_out);
			}
		}


		/* write attribute with array values */
		{
		AttributeData in;
		char *attr_name = "Double_spectrum";
		double arr[4];
		
		arr[0] = d_val;
		arr[1] = d_val + 1;
		arr[2] = d_val + 2;
		arr[3] = d_val + 3;
		
		in.data_type = DEV_DOUBLE;
		in.name      = attr_name;
		in.dim_x     = 4;
		in.dim_y     = 0;
		in.attr_data.double_arr.length   = 4;
		in.attr_data.double_arr.sequence = &(arr[0]);
		
		if (!tango_write_attribute (proxy, &in, &error))
			{
			tango_print_ErrorStack (&error);
			tango_free_ErrorStack (&error);
			}
		}

		/* write attributes with scalar values */
		
		{
		AttributeData in_att[2];
		AttributeDataList in_list;
			
		in_att[0].data_type = DEV_DOUBLE;
		in_att[0].name      = "Double_scalar";
		in_att[0].dim_x     = 1;
		in_att[0].dim_y     = 0;
		in_att[0].attr_data.double_arr.length   = 1;
		in_att[0].attr_data.double_arr.sequence = &d_val;
		
		in_att[1].data_type = DEV_LONG;
		in_att[1].name      = "Long_scalar";
		in_att[1].dim_x     = 1;
		in_att[1].dim_y     = 0;
		in_att[1].attr_data.long_arr.length   = 1;
		in_att[1].attr_data.long_arr.sequence = &l_val;		
		
		in_list.length = 2;
		in_list.sequence = &(in_att[0]);
		
		if (!tango_write_attributes (proxy, &in_list, &error))
			{
			tango_print_ErrorStack (&error);
			tango_free_ErrorStack (&error);
			}
		}

		/* read a list of attributes */
		
		{
		AttributeDataList out_list;
		VarStringArray attr_names;
		char *name_list[3];
		int i;
		
		name_list[0] = "Double_spectrum";
		name_list[1] = "Double_scalar";
		name_list[2] = "Long_scalar";
		
		attr_names.length   = 3;
		attr_names.sequence = &(name_list[0]);
		
		if (!tango_read_attributes (proxy, &attr_names, &out_list, &error))
			{
			tango_print_ErrorStack (&error);
			tango_free_ErrorStack (&error);
			}
		else
			{
			for (i=0; i<out_list.length; i++)
				{
				printf ("attr name = %s\n", out_list.sequence[i].name);
				
				if ( out_list.sequence[i].quality == ATTR_INVALID )
					{
					printf ("Data quality is INVALID!\n");
					}
				else
					{
					switch (out_list.sequence[i].data_type)
						{
						int j;
						
						case DEV_DOUBLE:
							for (j=0; j<out_list.sequence[i].dim_x; j++)
								{
								printf ("attr read value = %f\n", out_list.sequence[i].attr_data.double_arr.sequence[j]);
								}
							for (j=out_list.sequence[i].dim_x; j<out_list.sequence[i].attr_data.double_arr.length; j++)
								{
								printf ("attr set value  = %f\n", out_list.sequence[i].attr_data.double_arr.sequence[j]);
								}
							break;
							
						case DEV_LONG:
							for (j=0; j<out_list.sequence[i].dim_x; j++)
								{
								printf ("attr read value = %d\n", out_list.sequence[i].attr_data.long_arr.sequence[j]);
								}
							for (j=out_list.sequence[i].dim_x; j<out_list.sequence[i].attr_data.long_arr.length; j++)
								{
								printf ("attr set value  = %d\n", out_list.sequence[i].attr_data.long_arr.sequence[j]);
								}
							break;							
						}
					}
				printf ("\n");
				}
			
			tango_free_AttributeDataList (&out_list);
			}
		}
		
		/* write and read a list of device properties */
		
		{
		DbData prop_list;
		DbDatum props[2];
				
		prop_list.length   = 2;
		prop_list.sequence = &(props[0]);
					
		props[0].property_name = "double_property";
		props[0].data_type     = DEV_DOUBLE;
		props[0].prop_data.double_val = 123.45;
		
		props[1].property_name = "bool_property";
		props[1].data_type     = DEV_BOOLEAN;
		props[1].prop_data.bool_val = false;	
			
		/* write the properties */					
		if ( !tango_put_device_property (proxy, &prop_list, &error) )
			{
			tango_print_ErrorStack (&error);
			tango_free_ErrorStack (&error);
			}		
		else
			{
			/* read the properties */
			
			props[0].prop_data.double_val = 0;
			props[1].prop_data.bool_val   = true;				
				
			if ( !tango_get_device_property (proxy, &prop_list, &error) )
				{
				tango_print_ErrorStack (&error);
				tango_free_ErrorStack (&error);
				}		
			else
				{
				int i;
				for (i=0; i< prop_list.length; i++)
					{
					if ( prop_list.sequence[i].is_empty )
						{
						printf ("\nNo property value found for %s\n", 
					           prop_list.sequence[i].property_name);
						}
					else
						{
						if ( prop_list.sequence[i].wrong_data_type )
							{
							printf ("\nCannot convert property value to the requested data type for %s\n", 
									prop_list.sequence[i].property_name);
							}
						else
							{
							printf ("Property = %s\n", prop_list.sequence[i].property_name);
							
							if ( prop_list.sequence[i].data_type == DEV_BOOLEAN )
								{
								if ( prop_list.sequence[i].prop_data.bool_val == true )
									printf ("Value    = true\n");
								else
									printf ("Value    = false\n");
								}
							else
								{
								printf ("Value    = %f\n", prop_list.sequence[i].prop_data.double_val);
								}
							}
						}
					}
					
				tango_free_DbData (&prop_list); 
				}				
			}				
		}			
}
		
