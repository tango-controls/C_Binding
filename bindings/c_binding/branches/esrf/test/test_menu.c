static const char *RcsId = "$Id$\n$Name$";
/******************************************************************************
 * 
 * File       :	test_menu.c
 *
 * Project    :	C client interface to Tango
 * 
 * Description:	A test menu which uses the Tango C API
 *                Can be executed on any Tango device and demonstrates the available
 *                query functionality and basic command and attribute reading.
 *
 * Original   :	November 2007	
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
	void 			*proxy;
	ErrorStack error;

	char			cmd_string [80];
	long 			cmd;


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
	
	while (1)
		{
	   printf("\n\nSelect one of the following commands : \n\n");
	   printf("           1. Command list query\n");
	   printf("           2. Command info\n");
		printf("           3. Attribute list\n");
		printf("           4. Attribute list query\n");
		printf("           5. Attribute info\n\n");
		
		printf("           6. Get timeout\n");
		printf("           7. Set timeout\n");
		printf("           8. Get data source\n");		
		printf("           9. Set data source\n\n");
		
		printf("          10. Execute command without argin\n");
		printf("          11. Read attribute\n");
		
	   printf("\n           0. Quit \n\n");
	   printf("Execute command number ? ");


	   for( ; gets(cmd_string) == (char *)0 ; );
	   sscanf(cmd_string,"%d",&cmd);

	   if ( cmd != 0 )
	      {
	      printf 
			("\n\n**********************************************************\n");
	      }

	   switch (cmd) 
			{

	   	case (1) :
				{
				CommandInfoList cmd_list;			
				
				if ( !tango_command_list_query (proxy, &cmd_list, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					int i;
					
					printf ("\nNumber of commands = %d\n", cmd_list.length);
					for (i=0; i<cmd_list.length; i++)
						{
						printf ("cmd_name = %s\n",   cmd_list.sequence[i].cmd_name);
						printf ("in_type  = %s\n",   cmd_list.sequence[i].in_type_desc);
						printf ("out_type = %s\n\n", cmd_list.sequence[i].out_type_desc);
						}
						
					tango_free_CommandInfoList (&cmd_list);
					}
				}
				break;

	   	case (2) :
				{
				CommandInfo cmd_info;			
								
				printf("\nEnter a command name : ");
	   		for( ; gets(cmd_string) == (char *)0 ; );
				
				if ( !tango_command_query (proxy, cmd_string, &cmd_info, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					printf ("cmd_name = %s\n",   cmd_info.cmd_name);
					printf ("in_type  = %s\n",   cmd_info.in_type_desc);
					printf ("out_type = %s\n\n", cmd_info.out_type_desc);
					
					tango_free_CommandInfo (&cmd_info);
					}				
				}
				break;
				
	   	case (3) :
				{
				VarStringArray attr_names;			
								
				if ( !tango_get_attribute_list (proxy, &attr_names, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					int i;
					
					printf ("\nNumber of attributes = %d\n", attr_names.length);
					for (i=0; i<attr_names.length; i++)
						{
						printf ("[%d] = %s\n", i, attr_names.sequence[i]);
						}
											
					tango_free_VarStringArray (&attr_names);
					}				
				}
				break;				
		
	   	case (4) :
				{
				AttributeInfoList attr_list;			
				
				if ( !tango_attribute_list_query (proxy, &attr_list, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					int i;
					
					printf ("\nNumber of attributes = %d\n", attr_list.length);
					for (i=0; i<attr_list.length; i++)
						{
						printf ("name        = %s\n",  attr_list.sequence[i].name);
						printf ("description = %s\n",  attr_list.sequence[i].description);
						printf ("unit        = %s\n",  attr_list.sequence[i].unit);
						printf ("format      = %d : %d : %d\n\n",  attr_list.sequence[i].writable,
						attr_list.sequence[i].data_format, attr_list.sequence[i].data_type );						
						}
						
					tango_free_AttributeInfoList (&attr_list);
					}
				}
				break;
				
	   	case (5) :
				{
				AttributeInfoList attr_info;
				VarStringArray attr_names;
				char	*names_ptr[1];			
								
				printf("\nEnter an attribute name : ");
	   		for( ; gets(cmd_string) == (char *)0 ; );
				
				attr_names.length = 1;
				attr_names.sequence = names_ptr;
				*names_ptr = cmd_string;
				
				if ( !tango_get_attribute_config (proxy, &attr_names, &attr_info, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					printf ("name        = %s\n",  attr_info.sequence[0].name);
					printf ("description = %s\n",  attr_info.sequence[0].description);
					printf ("unit        = %s\n",  attr_info.sequence[0].unit);
					printf ("format      = %d : %d : %d\n\n",  attr_info.sequence[0].writable,
					attr_info.sequence[0].data_format, attr_info.sequence[0].data_type );						
					
					tango_free_AttributeInfoList (&attr_info);
					}				
				}
				break;

	   	case (6) :
				{
				int timeout = 0;
							
				if ( !tango_get_timeout_millis (proxy, &timeout, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					printf ("timeout = %d\n",  timeout);
					}				
				}
				break;

	   	case (7) :
				{
				int timeout = 0;
				
				printf("\nEnter timeout in milli seconds : ");
	   		for( ; gets(cmd_string) == (char *)0 ; );
				timeout = atol (cmd_string);
							
				if ( !tango_set_timeout_millis (proxy, timeout, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					printf ("timeout set = %d\n",  timeout);
					}				
				}
				break;

	   	case (8) :
				{
				DevSource source;
							
				if ( !tango_get_source (proxy, &source, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					switch (source)
						{
						case DEV:
							printf ("Data source = DEVICE\n");
							break;
						case CACHE:
							printf ("Data source = CACHE\n");
							break;
						case CACHE_DEV:
							printf ("Data source = First CACHE than DEVICE\n");
							break;							
						}
					}				
				}
				break;

	   	case (9) :
				{
				DevSource source;
				
				printf("\nEnter the data source (DEV, CACHE, CACHE_DEV) : ");
	   		for( ; gets(cmd_string) == (char *)0 ; );
				if ( strcmp(cmd_string, "DEV") == 0 )
					{
					source = DEV;
					}
				else
					{
					if ( strcmp(cmd_string, "CACHE") == 0 )
						{
						source = CACHE;
						}
					else
						{
						source = CACHE_DEV;
						}
					}
							
				if ( !tango_set_source (proxy, source, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					printf ("Data source set = %s\n", cmd_string);
					}				
				}
				break;


	   	case (10) :
				{
				CommandData cmd_in, cmd_out;
				
				printf("\nEnter a command name : ");
	   		for( ; gets(cmd_string) == (char *)0 ; );

				cmd_in.arg_type  = DEV_VOID;
				if (!tango_command_inout (proxy, cmd_string, &cmd_in, &cmd_out, &error))
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					switch (cmd_out.arg_type)
						{
						int i;
						
						case DEV_VOID:
							printf ("cmd result = void\n");
							break;
												
						case DEV_SHORT:
							printf ("cmd result = %d\n", cmd_out.cmd_data.short_val);
							break;

						case DEV_LONG:
							printf ("cmd result = %d\n", cmd_out.cmd_data.long_val);
							break;

						case DEV_FLOAT:
							printf ("cmd result = %f\n", cmd_out.cmd_data.float_val);
							break;
									
						case DEV_DOUBLE:
							printf ("cmd result = %f\n", cmd_out.cmd_data.double_val);
							break;
						
						case DEV_STATE:
							printf ("cmd result = %s\n", DevStateName[cmd_out.cmd_data.state_val]);
							break;
							
						case DEV_STRING:
							printf ("cmd result = %s\n", cmd_out.cmd_data.string_val);
							break;				


						case DEVVAR_SHORTARRAY:
							for (i=0; i<cmd_out.cmd_data.short_arr.length; i++)
								{
								printf ("cmd result = %d\n", cmd_out.cmd_data.short_arr.sequence[i]);
								}
							break;

						case DEVVAR_LONGARRAY:
							for (i=0; i<cmd_out.cmd_data.long_arr.length; i++)
								{
								printf ("cmd result = %d\n", cmd_out.cmd_data.long_arr.sequence[i]);
								}
							break;

						case DEVVAR_FLOATARRAY:
							for (i=0; i<cmd_out.cmd_data.long_arr.length; i++)
								{
								printf ("cmd result = %s\n", cmd_out.cmd_data.float_arr.sequence[i]);
								}
								break;
									
						case DEVVAR_DOUBLEARRAY:
							for (i=0; i<cmd_out.cmd_data.long_arr.length; i++)
								{
								printf ("cmd result = %f\n", cmd_out.cmd_data.double_arr.sequence[i]);
								}
							break;
													
						case DEVVAR_STRINGARRAY:
							for (i=0; i<cmd_out.cmd_data.long_arr.length; i++)
								{
								printf ("cmd result = %s\n", cmd_out.cmd_data.string_arr.sequence[i]);
								}
							break;

						
						default:
							printf ("Printing for the cmd data type is not implemented in this menu!\n");
							break;							
						}
					
					tango_free_CommandData (&cmd_out);
					}				
				}
				break;


	   	case (11) :
				{
				AttributeData attr_out;
				
				printf("\nEnter an attribute name : ");
	   		for( ; gets(cmd_string) == (char *)0 ; );

				if (!tango_read_attribute (proxy, cmd_string, &attr_out, &error))
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					}		
				else
					{
					if ( attr_out.quality == ATTR_INVALID )
						{
						printf ("Attribute data is INVALID!\n");
						}
					else
						{						
						switch (attr_out.data_type)
							{
							int i;
							
							case DEV_BOOLEAN:
								for (i=0; i<attr_out.dim_x; i++)
									{
									printf ("attr read value = %d\n", attr_out.attr_data.bool_arr.sequence[i]);
									}
								for (i=attr_out.dim_x; i<attr_out.attr_data.bool_arr.length; i++)
									{
									printf ("attr set value  = %d\n", attr_out.attr_data.bool_arr.sequence[i]);
									}
								break;							
							

							case DEV_UCHAR:
								for (i=0; i<attr_out.dim_x; i++)
									{
									printf ("attr read value = %d\n", attr_out.attr_data.char_arr.sequence[i]);
									}
								for (i=attr_out.dim_x; i<attr_out.attr_data.char_arr.length; i++)
									{
									printf ("attr set value  = %d\n", attr_out.attr_data.char_arr.sequence[i]);
									}
								break;

							case DEV_SHORT:
								for (i=0; i<attr_out.dim_x; i++)
									{
									printf ("attr read value = %d\n", attr_out.attr_data.short_arr.sequence[i]);
									}
								for (i=attr_out.dim_x; i<attr_out.attr_data.short_arr.length; i++)
									{
									printf ("attr set value  = %d\n", attr_out.attr_data.short_arr.sequence[i]);
									}
								break;

							case DEV_LONG:
								for (i=0; i<attr_out.dim_x; i++)
									{
									printf ("attr read value = %d\n", attr_out.attr_data.long_arr.sequence[i]);
									}
								for (i=attr_out.dim_x; i<attr_out.attr_data.long_arr.length; i++)
									{
									printf ("attr set value  = %d\n", attr_out.attr_data.long_arr.sequence[i]);
									}
								break;
	
							case DEV_FLOAT:
								for (i=0; i<attr_out.dim_x; i++)
									{
									printf ("attr read value = %f\n", attr_out.attr_data.float_arr.sequence[i]);
									}
								for (i=attr_out.dim_x; i<attr_out.attr_data.float_arr.length; i++)
									{
									printf ("attr set value  = %f\n", attr_out.attr_data.float_arr.sequence[i]);
									}
								break;
								
							case DEV_DOUBLE:
								for (i=0; i<attr_out.dim_x; i++)
									{
									printf ("attr read value = %f\n", attr_out.attr_data.double_arr.sequence[i]);
									}
								for (i=attr_out.dim_x; i<attr_out.attr_data.double_arr.length; i++)
									{
									printf ("attr set value  = %f\n", attr_out.attr_data.double_arr.sequence[i]);
									}
								break;
								
							case DEV_STATE:
								for (i=0; i<attr_out.dim_x; i++)
									{
									printf ("attr read value = %s\n", DevStateName[attr_out.attr_data.state_arr.sequence[i]]);
									}
								for (i=attr_out.dim_x; i<attr_out.attr_data.state_arr.length; i++)
									{
									printf ("attr set value  = %s\n", DevStateName[attr_out.attr_data.state_arr.sequence[i]]);
									}
								break;
								
							case DEV_STRING:
								for (i=0; i<attr_out.dim_x; i++)
									{
									printf ("attr read value = %s\n", attr_out.attr_data.string_arr.sequence[i]);
									}
								for (i=attr_out.dim_x; i<attr_out.attr_data.string_arr.length; i++)
									{
									printf ("attr set value  = %s\n", attr_out.attr_data.string_arr.sequence[i]);
									}
								break;																							
								
							default:
								printf ("Printing for the attribute data type is not implemented in this menu!\n");
								break;
							}							
						}
					
					tango_free_AttributeData (&attr_out);
					}				
				}
				break;


							
	   	case (0) : 
				/*
		 			   *  free the connection to the device
		 				*/
				if ( !tango_delete_device_proxy (&proxy, &error) )
					{
					tango_print_ErrorStack (&error);
					tango_free_ErrorStack (&error);
					exit (1);
					}
					
	        	exit(0);
				break;


	   	default : 
			printf ("Unknown Choice !!!!!!!\n");
			break;
	   	}
		
		printf (
		"**********************************************************\n");			
		}
}
