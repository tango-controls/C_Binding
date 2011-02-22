// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Functions.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _EXTERNAL_FUNCTIONS_H_
#define _EXTERNAL_FUNCTIONS_H_

//=============================================================================
// DEFINEs
//=============================================================================
#define XDK_Functions XFUNC

//=============================================================================
// EXTERNAL FUNCTIONS (SINGLETON)
//=============================================================================
class XDK_Functions
{
public:
    static long call_mode (long _xfunc_id);
    // Select exec mode for <_xfunc_id>.
    
    static int exec (long _xfunc_id, void * _xfunc_params);
    // Execute <_xfunc_id> (msg oriented exec mode only). 

private:
    //----------------------------------------------------------
    // TANGO API BINDING
    //----------------------------------------------------------
    static int tango_open_device (void* _p);
    static int tango_close_device (void* _p);
    //- Tango::DeviceProxy ------------------------------------- 
    static int tango_command_inout (void* _p);
    static int tango_read_attribute (void* _p);
    static int tango_read_attributes (void* _p);
    static int tango_write_attribute (void* _p);
    static int tango_write_attributes (void* _p);
    static int tango_get_dev_status (void* _p);
    static int tango_ping_dev (void* _p);
    static int tango_set_dev_timeout (void* _p);
    static int tango_get_dev_timeout (void* _p);
    static int tango_get_dev_cmd_list (void* _p);
    static int tango_get_dev_attr_list (void* _p);
    static int tango_get_dev_black_box (void* _p);
    static int tango_error_stack (void* _p);
    //- Tango::Monitoring -------------------------------------- 
    static int tango_add_attr_monitor (void* _p);
    static int tango_remove_attr_monitor (void* _p);
    static int tango_suspend_attr_monitor (void* _p);
    static int tango_resume_attr_monitor (void* _p);
    static int tango_set_attr_monitor_period (void* _p);
    //- Tango::Optimized processing ----------------------------
    static int compute_image_projection (void *_p);

    //----------------------------------------------------------
    // METHODES INTERDITES
    //----------------------------------------------------------
    XDK_Functions ();
    XDK_Functions (const XDK_Functions&);
    void operator= (const XDK_Functions&); 
    virtual ~XDK_Functions ();
};

#endif // _EXTERNAL_FUNCTIONS_H_
   
   