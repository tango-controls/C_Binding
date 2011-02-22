static const char *RcsId = "$Id$\n$Name$";
/******************************************************************************
 * 
 * File       :	c_tango_command.c
 *
 * Project    :	C client interface to Tango
 * 
 * Description:	Interface functions to access Tango commands
 *
 * Original   :	November 2007	
 *	
 * $Author$
 *
 * $Log$
 * Revision 1.5  2007/12/20 07:57:02  jensmeyer
 * Corrected file headers
 *
 * Revision 1.4  2007/12/18 17:26:20  jensmeyer
 * Added new file for database access and corrected bugs
 *
 * Revision 1.3  2007/12/12 14:20:50  jensmeyer
 * Added doxygen documentation headers and commented code
 *
 * Revision 1.2  2007/12/07 16:05:15  jensmeyer
 * Some name changes to be comaptible with Taco
 *
 * Revision 1.1.1.1  2007/12/05 15:05:04  jensmeyer
 * Tango C language binding
 *
 ******************************************************************************/ 

#include <c_tango.h>
#include <tango.h>

void translate_exception (Tango::DevFailed& tango_exception, ErrorStack *error);
static void convert_cmd_query (Tango::CommandInfo& tango_cmd_info, CommandInfo *cmd_info);

/********************************/
/* External interface functions */
/********************************/

bool tango_command_inout (void *proxy, char *cmd_name, CommandData *argin, 
                          CommandData *argout, ErrorStack *error)
{
	Tango::DeviceData  cmd_in;
	Tango::DeviceData  cmd_out;
	Tango::DeviceProxy *dev;
	
	try
		{
		dev = (Tango::DeviceProxy *) proxy;
		
		/* convert the input argument */
		
		switch (argin->arg_type)
			{
			case DEV_VOID:
				break;
		
			case DEV_BOOLEAN:
				cmd_in << argin->cmd_data.bool_val;
				break;				
								
			case DEV_SHORT:
				cmd_in << argin->cmd_data.short_val;
				break;
				
			case DEV_USHORT:
				cmd_in << argin->cmd_data.ushort_val;
				break;								
		
			case DEV_LONG:
				cmd_in << (Tango::DevLong) argin->cmd_data.long_val;
				break;				

			case DEV_ULONG:
				cmd_in << (Tango::DevULong) argin->cmd_data.ulong_val;
				break;				
			
			case DEV_LONG64:
				cmd_in << (Tango::DevLong64) argin->cmd_data.long64_val;
				break;				

			case DEV_ULONG64:
				cmd_in << (Tango::DevULong64) argin->cmd_data.ulong64_val;
				break;			
			
			case DEV_FLOAT:
				cmd_in << argin->cmd_data.float_val;
				break;
				
			case DEV_DOUBLE:
				cmd_in << argin->cmd_data.double_val;
				break;
				
			case DEV_STRING:
			case CONST_DEV_STRING:
				cmd_in << argin->cmd_data.string_val;
				break;
				
			case DEVVAR_CHARARRAY:
				{
				vector<unsigned char> char_arr(argin->cmd_data.char_arr.length);
				
				for (int i=0; i<argin->cmd_data.char_arr.length; i++)
					{
					char_arr[i] = (unsigned char) argin->cmd_data.char_arr.sequence[i];
					}
					
				cmd_in << char_arr;				
				break;
				}
				
			case DEVVAR_SHORTARRAY:
				{
				vector<short> short_arr(argin->cmd_data.short_arr.length);
				
				for (int i=0; i<argin->cmd_data.short_arr.length; i++)
					{
					short_arr[i] = (short) argin->cmd_data.short_arr.sequence[i];
					}
					
				cmd_in << short_arr;				
				break;
				}
				
			case DEVVAR_USHORTARRAY:
				{
				vector<unsigned short> ushort_arr(argin->cmd_data.ushort_arr.length);
				
				for (int i=0; i<argin->cmd_data.ushort_arr.length; i++)
					{
					ushort_arr[i] = (unsigned short) argin->cmd_data.ushort_arr.sequence[i];
					}
					
				cmd_in << ushort_arr;				
				break;
				}
				
			case DEVVAR_LONGARRAY:
				{
				vector<Tango::DevLong> long_arr(argin->cmd_data.long_arr.length);
				
				for (int i=0; i<argin->cmd_data.long_arr.length; i++)
					{
					long_arr[i] = (int) argin->cmd_data.long_arr.sequence[i];
					}
					
				cmd_in << long_arr;				
				break;
				}
				
			case DEVVAR_ULONGARRAY:
				{
				vector<Tango::DevULong> ulong_arr(argin->cmd_data.ulong_arr.length);
				
				for (int i=0; i<argin->cmd_data.ulong_arr.length; i++)
					{
					ulong_arr[i] = (unsigned int) argin->cmd_data.ulong_arr.sequence[i];
					}
					
				cmd_in << ulong_arr;				
				break;
				}
				
			case DEVVAR_LONG64ARRAY:				
				{
				vector<Tango::DevLong64> long64_arr(argin->cmd_data.long64_arr.length);
				
				for (int i=0; i<argin->cmd_data.long64_arr.length; i++)
					{
					long64_arr[i] = (Tango::DevLong64) argin->cmd_data.long64_arr.sequence[i];
					}
					
				cmd_in << long64_arr;				
				break;
				}
				
			case DEVVAR_ULONG64ARRAY:			
				{
				vector<Tango::DevULong64> ulong64_arr(argin->cmd_data.ulong64_arr.length);
				
				for (int i=0; i<argin->cmd_data.ulong64_arr.length; i++)
					{
					ulong64_arr[i] = (Tango::DevULong64) argin->cmd_data.ulong64_arr.sequence[i];
					}
					
				cmd_in << ulong64_arr;				
				break;																						
				}
				
			case DEVVAR_FLOATARRAY:
				{
				vector<float> float_arr(argin->cmd_data.float_arr.length);
				
				for (int i=0; i<argin->cmd_data.float_arr.length; i++)
					{
					float_arr[i] = (float) argin->cmd_data.float_arr.sequence[i];
					}
					
				cmd_in << float_arr;				
				break;				
				}
				
			case DEVVAR_DOUBLEARRAY:
				{
				vector<double> double_arr(argin->cmd_data.double_arr.length);
				
				for (int i=0; i<argin->cmd_data.double_arr.length; i++)
					{
					double_arr[i] = (double) argin->cmd_data.double_arr.sequence[i];
					}
					
				cmd_in << double_arr;				
				break;
				}
				
			case DEVVAR_STRINGARRAY:
				{
				vector<string> string_arr(argin->cmd_data.string_arr.length);
				
				for (int i=0; i<argin->cmd_data.string_arr.length; i++)
					{
					string_arr[i] = argin->cmd_data.string_arr.sequence[i];
					}
					
				cmd_in << string_arr;				
				break;
				}				
			
			default:
				Tango::Except::throw_exception 
						((const char *)"Data type error",
	                (const char *)"The requested data type is not implemented for command writing!",
	                (const char *)"c_tango_command.c::tango_command_inout()");
				break;			
			}
		
		
		
		/* treat the void case! */
		
		if ( argin->arg_type == DEV_VOID )
			{
			cmd_out = dev->command_inout(cmd_name);
			}
		else
			{
			cmd_out = dev->command_inout(cmd_name, cmd_in);
			}
		
		
		
		/* Treat the void case! */
		
		try
			{
			cmd_out.is_empty();
			argout->arg_type = (TangoDataType) cmd_out.get_type();
			}
		catch (Tango::DevFailed &e)
			{
			argout->arg_type = DEV_VOID;
			}
		
		/* convert the output argument */
		
		switch (argout->arg_type)
			{
			case DEV_VOID:
				break;

			case DEV_BOOLEAN:
				cmd_out >> argout->cmd_data.bool_val;
				break;				
											
			case DEV_SHORT:
				cmd_out >> argout->cmd_data.short_val;
				break;
				
			case DEV_USHORT:
				cmd_out >> argout->cmd_data.ushort_val;
				break;								
		
			case DEV_LONG:
				{
				Tango::DevLong long_val;
				cmd_out >> long_val;
				argout->cmd_data.long_val = long_val;
				break;
				}				

			case DEV_ULONG:
				{
				Tango::DevULong ulong_val;
				cmd_out >> ulong_val;
				argout->cmd_data.ulong_val = ulong_val;
				break;
				}				
			
			case DEV_LONG64:
				{
				Tango::DevLong64 long64_val;
				cmd_out >> long64_val;
				argout->cmd_data.long64_val = long64_val;
				break;
				}				

			case DEV_ULONG64:
				{
				Tango::DevULong64 ulong64_val;
				cmd_out >> ulong64_val;
				argout->cmd_data.ulong64_val = ulong64_val;
				break;
				}		
			
			case DEV_FLOAT:
				cmd_out >> argout->cmd_data.float_val;
				break;
				
			case DEV_DOUBLE:
				cmd_out >> argout->cmd_data.double_val;
				break;
			
			case DEV_STATE:
				Tango::DevState state_val;
				cmd_out >> state_val;
				argout->cmd_data.state_val = (TangoDevState) state_val; 
				break;
				
			case DEV_STRING:
			case CONST_DEV_STRING:
				{
				string string_val;
				cmd_out >> string_val;
				argout->cmd_data.string_val = new char[string_val.length() + 1];
				sprintf (argout->cmd_data.string_val, "%s", string_val.c_str());
				break;
				}
				
			case DEVVAR_CHARARRAY:
				{
				const Tango::DevVarCharArray *char_seq;
				int			               nb_data;

				cmd_out >> char_seq;
				nb_data =  char_seq->length();

				argout->cmd_data.char_arr.length   = nb_data;	
				argout->cmd_data.char_arr.sequence = new unsigned char[nb_data];

				memcpy ( argout->cmd_data.char_arr.sequence,
				         char_seq->get_buffer(), 
						   (sizeof(unsigned char) * nb_data) );						
				break;
				}					

			case DEVVAR_SHORTARRAY:
				{
				const Tango::DevVarShortArray *short_seq;
				int			               nb_data;

				cmd_out >> short_seq;
				nb_data =  short_seq->length();

				argout->cmd_data.short_arr.length   = nb_data;	
				argout->cmd_data.short_arr.sequence = new short[nb_data];

				memcpy ( argout->cmd_data.short_arr.sequence,
				         short_seq->get_buffer(), 
						   (sizeof(short) * nb_data) );						
				break;
				}

			case DEVVAR_USHORTARRAY:
				{
				const Tango::DevVarUShortArray *ushort_seq;
				int			               nb_data;

				cmd_out >> ushort_seq;
				nb_data =  ushort_seq->length();

				argout->cmd_data.ushort_arr.length   = nb_data;	
				argout->cmd_data.ushort_arr.sequence = new unsigned short[nb_data];

				memcpy ( argout->cmd_data.ushort_arr.sequence,
				         ushort_seq->get_buffer(), 
						   (sizeof(unsigned short) * nb_data) );						
				break;
				}				

			case DEVVAR_LONGARRAY:
				{
				const Tango::DevVarLongArray *long_seq;
				int			               nb_data;

				cmd_out >> long_seq;
				nb_data =  long_seq->length();

				argout->cmd_data.long_arr.length   = nb_data;	
				argout->cmd_data.long_arr.sequence = new int[nb_data];

				memcpy ( argout->cmd_data.long_arr.sequence,
				         long_seq->get_buffer(), 
						   (sizeof(int) * nb_data) );						
				break;
				}

			case DEVVAR_ULONGARRAY:
				{
				const Tango::DevVarULongArray *ulong_seq;
				int			               nb_data;

				cmd_out >> ulong_seq;
				nb_data =  ulong_seq->length();

				argout->cmd_data.ulong_arr.length   = nb_data;	
				argout->cmd_data.ulong_arr.sequence = new unsigned int[nb_data];

				memcpy ( argout->cmd_data.ulong_arr.sequence,
				         ulong_seq->get_buffer(), 
						   (sizeof(unsigned int) * nb_data) );						
				break;
				}
				
			case DEVVAR_LONG64ARRAY:
				{
				const Tango::DevVarLong64Array *long64_seq;
				int			               nb_data;

				cmd_out >> long64_seq;
				nb_data =  long64_seq->length();

				argout->cmd_data.long64_arr.length   = nb_data;	
				argout->cmd_data.long64_arr.sequence = new Tango::DevLong64[nb_data];

				memcpy ( argout->cmd_data.long64_arr.sequence,
				         long64_seq->get_buffer(), 
						   (sizeof(Tango::DevLong64) * nb_data) );						
				break;
				}

			case DEVVAR_ULONG64ARRAY:
				{
				const Tango::DevVarULong64Array *ulong64_seq;
				int			               nb_data;

				cmd_out >> ulong64_seq;
				nb_data =  ulong64_seq->length();

				argout->cmd_data.ulong64_arr.length   = nb_data;	
				argout->cmd_data.ulong64_arr.sequence = new Tango::DevULong64[nb_data];

				memcpy ( argout->cmd_data.ulong64_arr.sequence,
				         ulong64_seq->get_buffer(), 
						   (sizeof(Tango::DevULong64) * nb_data) );						
				break;
				}				
				
			case DEVVAR_FLOATARRAY:
				{
				const Tango::DevVarFloatArray *float_seq;
				int			               nb_data;

				cmd_out >> float_seq;
				nb_data =  float_seq->length();

				argout->cmd_data.float_arr.length   = nb_data;	
				argout->cmd_data.float_arr.sequence = new float[nb_data];

				memcpy ( argout->cmd_data.float_arr.sequence,
				         float_seq->get_buffer(), 
						   (sizeof(float) * nb_data) );						
				break;
				}			
				
			case DEVVAR_DOUBLEARRAY:
				{
				const Tango::DevVarDoubleArray *double_seq;
				int			               nb_data;

				cmd_out >> double_seq;
				nb_data =  double_seq->length();

				argout->cmd_data.double_arr.length   = nb_data;	
				argout->cmd_data.double_arr.sequence = new double[nb_data];

				memcpy ( argout->cmd_data.double_arr.sequence,
				         double_seq->get_buffer(), 
						   (sizeof(double) * nb_data) );						
				break;
				}
				
			case DEVVAR_STRINGARRAY:
				{
				vector<string>	string_vect;
				int nb_data;


				cmd_out >> string_vect;
				nb_data = string_vect.size();

				/* allocate sequence */
				argout->cmd_data.string_arr.sequence = (char **) calloc(nb_data, sizeof(char *));;
				argout->cmd_data.string_arr.length   = nb_data;

				/* allocate strings and copy data */
				for (int i=0 ; i<nb_data ; i++)
					{
					argout->cmd_data.string_arr.sequence[i] = new char[string_vect[i].length() + 1];
					sprintf (argout->cmd_data.string_arr.sequence[i], "%s", 
				         	string_vect[i].c_str());
					}				  
				break;										
				}
			
			default:
				Tango::Except::throw_exception 
						((const char *)"Data type error",
	                (const char *)"The requested data type is not implemented for command reading!",
	                (const char *)"c_tango_command.c::tango_command_inout()");
				break;			
			}
		}
		
	catch (Tango::DevFailed &tango_exception)
		{
		translate_exception (tango_exception, error);		
		return false;
		}
				
	return true;
}



void tango_free_CommandData (CommandData *command_data)
{	
	switch (command_data->arg_type)
		{
		case DEV_STRING:
			free (command_data->cmd_data.string_val);
			break;
		
		case DEVVAR_CHARARRAY:
			free (command_data->cmd_data.char_arr.sequence);
			
			command_data->cmd_data.char_arr.sequence = NULL;
			command_data->cmd_data.char_arr.length = 0;
			break;
											
		case DEVVAR_SHORTARRAY:
			free (command_data->cmd_data.short_arr.sequence);
			
			command_data->cmd_data.short_arr.sequence = NULL;
			command_data->cmd_data.short_arr.length = 0;
			break;
	
		case DEVVAR_USHORTARRAY:
			free (command_data->cmd_data.ushort_arr.sequence);
			
			command_data->cmd_data.ushort_arr.sequence = NULL;
			command_data->cmd_data.ushort_arr.length = 0;
			break;	
			
		case DEVVAR_LONGARRAY:
			free (command_data->cmd_data.long_arr.sequence);
			
			command_data->cmd_data.long_arr.sequence = NULL;
			command_data->cmd_data.long_arr.length = 0;
			break;			
	
		case DEVVAR_ULONGARRAY:
			free (command_data->cmd_data.ulong_arr.sequence);
			
			command_data->cmd_data.ulong_arr.sequence = NULL;
			command_data->cmd_data.ulong_arr.length = 0;
			break;	

		case DEVVAR_LONG64ARRAY:
			free (command_data->cmd_data.long64_arr.sequence);
			
			command_data->cmd_data.long64_arr.sequence = NULL;
			command_data->cmd_data.long64_arr.length = 0;
			break;			
	
		case DEVVAR_ULONG64ARRAY:
			free (command_data->cmd_data.ulong64_arr.sequence);
			
			command_data->cmd_data.ulong64_arr.sequence = NULL;
			command_data->cmd_data.ulong64_arr.length = 0;
			break;
	
		case DEVVAR_FLOATARRAY:
			free (command_data->cmd_data.float_arr.sequence);
			
			command_data->cmd_data.float_arr.sequence = NULL;
			command_data->cmd_data.float_arr.length = 0;
			break;	
					
		case DEVVAR_DOUBLEARRAY:
			free (command_data->cmd_data.double_arr.sequence);
			
			command_data->cmd_data.double_arr.sequence = NULL;
			command_data->cmd_data.double_arr.length = 0;
			break;
			
		case DEVVAR_STRINGARRAY:
			for (int i=0; i<command_data->cmd_data.string_arr.length; i++ )
				{
				free (command_data->cmd_data.string_arr.sequence[i]);
				}
				
			free (command_data->cmd_data.string_arr.sequence);
			command_data->cmd_data.string_arr.sequence = NULL;
			command_data->cmd_data.string_arr.length = 0;
			break;
			}
}



bool tango_command_query (void *proxy, char *cmd_name, CommandInfo *cmd_info, ErrorStack *error)
{
	Tango::DeviceProxy *dev;
	Tango::CommandInfo tango_cmd_info;
	
	try
		{
		dev = (Tango::DeviceProxy *) proxy;
		tango_cmd_info = dev->command_query (cmd_name);
		
		convert_cmd_query (tango_cmd_info, cmd_info);		
		}
	
	catch (Tango::DevFailed &tango_exception)
		{
		translate_exception (tango_exception, error);		
		return false;
		}
		
	return true;
}


bool tango_command_list_query (void *proxy, CommandInfoList *cmd_info_list, ErrorStack *error)
{
	Tango::DeviceProxy *dev;
	vector<Tango::CommandInfo> *tango_cmd_info_list;
	
	try
		{
		dev = (Tango::DeviceProxy *) proxy;
		tango_cmd_info_list = dev->command_list_query ();
		
		/* allocate the  CommandInfoList for the number of commands returned */
		cmd_info_list->length   = tango_cmd_info_list->size();
		cmd_info_list->sequence = new CommandInfo[cmd_info_list->length];
		
		/* loop over all returned commands and convert the data */
		for (int i=0; i < tango_cmd_info_list->size(); i++)
			{
			convert_cmd_query ((*tango_cmd_info_list)[i], &(cmd_info_list->sequence[i]));
			}				
		}
	
	catch (Tango::DevFailed &tango_exception)
		{
		translate_exception (tango_exception, error);		
		return false;
		}
		
	return true;
}


void tango_free_CommandInfo (CommandInfo *command_info)
{
	free (command_info->cmd_name);
	command_info->cmd_name = NULL;
	
	free (command_info->in_type_desc);
	command_info->in_type_desc = NULL;
	
	free (command_info->out_type_desc);
	command_info->out_type_desc = NULL;		
}


void tango_free_CommandInfoList (CommandInfoList *command_info_list)
{
	for (int i=0; i<command_info_list->length; i++)
		{
		tango_free_CommandInfo ( &(command_info_list->sequence[i]) );
		}	
	
	free (command_info_list->sequence);
	command_info_list->sequence = NULL;
	command_info_list->length   = 0;
}


/*************************************/
/* internal library helper functions */
/*************************************/


static void convert_cmd_query (Tango::CommandInfo& tango_cmd_info, CommandInfo *cmd_info)
{
	/* allocate command name */
	cmd_info->cmd_name = new char[tango_cmd_info.cmd_name.length() + 1];
	sprintf (cmd_info->cmd_name, "%s", tango_cmd_info.cmd_name.c_str());

	/* allocate command data descriptions */
	cmd_info->in_type_desc = new char[tango_cmd_info.in_type_desc.length() + 1];
	sprintf (cmd_info->in_type_desc, "%s", tango_cmd_info.in_type_desc.c_str());

	cmd_info->out_type_desc = new char[tango_cmd_info.out_type_desc.length() + 1];
	sprintf (cmd_info->out_type_desc, "%s", tango_cmd_info.out_type_desc.c_str());

	cmd_info->cmd_tag = tango_cmd_info.cmd_tag;
	cmd_info->in_type = tango_cmd_info.in_type;
	cmd_info->out_type = tango_cmd_info.out_type;
	cmd_info->disp_level = (DispLevel) tango_cmd_info.disp_level;
}
