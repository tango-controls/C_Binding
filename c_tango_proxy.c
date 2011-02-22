static const char *RcsId = "$Id$\n$Name$";
/******************************************************************************
 * 
 * File       :	c_tango_proxy.c
 *
 * Project    :	C client interface to Tango
 * 
 * Description:	Interface functions to access Tango devices
 *
 * Original   :	November 2007	
 *	
 * $Author$
 *
 * $Log$
 * Revision 1.3  2007/12/03 14:46:51  meyer
 * Added timeout and data source functions.
 *
 * Revision 1.2  2007/12/03 07:25:31  meyer
 * Function cores prepared
 *
 * Revision 1.1  2007/11/29 14:01:22  meyer
 * Initial revision
 *
 *
 ******************************************************************************/ 

#include <c_tango.h>
#include <tango.h>

void translate_exception (Tango::DevFailed& tango_exception, ErrorStack *error);

/********************************/
/* External interface functions */
/********************************/

/**@name Command related functions
 * Functions to execute commands on Tango devices
 */


/* Create the device proxy */

bool tango_create_device_proxy (char *dev_name, void **proxy, ErrorStack *error)
{
	try
		{
		Tango::DeviceProxy *dev = new Tango::DeviceProxy(dev_name);
		*proxy = (void *) dev;
		}
	
	catch (Tango::DevFailed &e)
		{
		translate_exception (e, error);
		return false;
		}
			
	return true;
}


/* delete the device proxy */

bool tango_delete_device_proxy (void **proxy, ErrorStack *error)
{
	Tango::DeviceProxy *dev;
	
	try
		{
		dev = (Tango::DeviceProxy *) *proxy;
		delete dev;
		}
		
	catch (Tango::DevFailed &tango_exception)
		{
		translate_exception (tango_exception, error);
		return false;
		}
	
	*proxy = NULL;
	return true;
}



bool tango_set_timeout_millis (void *proxy, int millis, ErrorStack *error)
{
	Tango::DeviceProxy *dev;
	
	try
		{
		dev = (Tango::DeviceProxy *) proxy;
		dev->set_timeout_millis(millis);
		}
		
	catch (Tango::DevFailed &tango_exception)
		{
		translate_exception (tango_exception, error);
		return false;
		}
		
	return true;
}

bool tango_get_timeout_millis (void *proxy, int *millis, ErrorStack *error)
{
	Tango::DeviceProxy *dev;
	
	try
		{
		dev = (Tango::DeviceProxy *) proxy;
		*millis = dev->get_timeout_millis();
		}
		
	catch (Tango::DevFailed &tango_exception)
		{
		translate_exception (tango_exception, error);
		return false;
		}
		
	return true;
}

bool tango_set_source (void *proxy, DevSource source, ErrorStack *error)
{
	Tango::DeviceProxy *dev;
	
	try
		{
		dev = (Tango::DeviceProxy *) proxy;
		dev->set_source ((Tango::DevSource)source);
		}
		
	catch (Tango::DevFailed &tango_exception)
		{
		translate_exception (tango_exception, error);
		return false;
		}
		
	return true;
}

bool tango_get_source (void *proxy, DevSource *source, ErrorStack *error)
{
	Tango::DeviceProxy *dev;
	
	try
		{
		dev = (Tango::DeviceProxy *) proxy;
		*source = (DevSource) dev->get_source();
		}
		
	catch (Tango::DevFailed &tango_exception)
		{
		translate_exception (tango_exception, error);
		return false;
		}
		
	return true;
}





void tango_free_ErrorStack (ErrorStack *error)
{
	for (int i=0; i<error->length; i++)
		{
		free(error->sequence[i].desc);
		free(error->sequence[i].reason);
		free(error->sequence[i].origin);
		}	
	
	free (error->sequence);
	error->sequence = NULL;
	error->length   = 0;
}


void tango_print_ErrorStack (ErrorStack *error)
{
	printf ("\nTango exception:\n");
	for (int i=0; i<error->length; i++)
		{
		printf ("Severity    : %d\n", error->sequence[i].severity);
		printf ("Reason      : %s\nDescription : %s\nOrigin      : %s\n\n",
					error->sequence[i].reason,
					error->sequence[i].desc,
					error->sequence[i].origin);
		}
}


/*****************************/
/* Internal helper functions */
/*****************************/

void translate_exception (Tango::DevFailed& tango_exception, ErrorStack *error)
{
		/* allocate error stack */
		error->length   = tango_exception.errors.length();
		error->sequence = new DevFailed[error->length];
		
		/* copy the full tango error list */
   	for (int i=0; i<tango_exception.errors.length(); i++)
   		{
			error->sequence[i].desc = new char[strlen(tango_exception.errors[i].desc.in()) + 1];
			sprintf (error->sequence[i].desc, "%s", tango_exception.errors[i].desc.in());
			
			error->sequence[i].reason = new char[strlen(tango_exception.errors[i].reason.in()) + 1];
			sprintf (error->sequence[i].reason, "%s", tango_exception.errors[i].reason.in());

			error->sequence[i].origin = new char[strlen(tango_exception.errors[i].origin.in()) + 1];
			sprintf (error->sequence[i].origin, "%s", tango_exception.errors[i].origin.in());
			
			error->sequence[i].severity = (ErrSeverity) tango_exception.errors[i].severity;
			}
}

