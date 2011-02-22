//=============================================================================

// XOP SPECIFIC RESOURCE : XOPF 1100 - XOP EXTERNAL FUNCTIONS DEFINITION

//=============================================================================

// 

// XFUNC input parameters types : 

//  - NT_FP64

//  - HSTRING_TYPE

//  - WAVE_TYPE

//

//=============================================================================

1100 XOPF

BEGIN

    //-- kTANGO_OPEN_DEV

    "tango_open_device\0",

    F_EXTERNAL,

    NT_FP64,

      HSTRING_TYPE, // device

      0,

    //-- kTANGO_CLOSE_DEV

    "tango_close_device\0",

    F_EXTERNAL,

    NT_FP64,

      HSTRING_TYPE, // device

      0,

    //-- kTANGO_CMD_INOUT

    "tango_command_inout\0",

    F_EXTERNAL,

    NT_FP64,        

      HSTRING_TYPE, // device

      HSTRING_TYPE, // command

      HSTRING_TYPE, // argin

      HSTRING_TYPE, // argout

      0,

    //-- kTANGO_READ_ATTR

    "tango_read_attribute\0",

    F_EXTERNAL,

    NT_FP64,        

      HSTRING_TYPE, // device

      HSTRING_TYPE, // attribute

      HSTRING_TYPE, // argout

      0,

    //-- kTANGO_READ_ATTRS

    "tango_read_attributes\0",

    F_EXTERNAL,

    NT_FP64,        

      HSTRING_TYPE, // device

      HSTRING_TYPE, // attributes and argouts

      0,

    //-- kTANGO_WRITE_ATTR

    "tango_write_attribute\0",

    F_EXTERNAL,

    NT_FP64,        

      HSTRING_TYPE, // device

      HSTRING_TYPE, // attribute

      HSTRING_TYPE, // argin

      0,

    //-- kTANGO_WRITE_ATTRS

    "tango_write_attributes\0",

    F_EXTERNAL,

    NT_FP64,        

      HSTRING_TYPE, // device

      HSTRING_TYPE, // attributes and argins

      0,

    //-- kTANGO_GET_DEV_STATUS

    "tango_get_dev_status\0",

    F_EXTERNAL,

    HSTRING_TYPE, 

      HSTRING_TYPE, // device

      0,

    //-- kTANGO_PING_DEV

    "tango_ping_device\0",

    F_EXTERNAL,

    NT_FP64,

      HSTRING_TYPE, // device

      0,

    //-- kTANGO_SET_DEV_TMO

    "tango_set_dev_timeout\0",

    F_EXTERNAL,

    NT_FP64,

      HSTRING_TYPE, // device

      NT_FP64,      // timeout

      0,

    //-- kTANGO_GET_DEV_TMO

    "tango_get_dev_timeout\0",

    F_EXTERNAL,

    NT_FP64,

      HSTRING_TYPE, // device

      0,

    //-- kTANGO_CMD_LIST

    "tango_get_dev_cmd_list\0",

    F_EXTERNAL,

    NT_FP64,

      HSTRING_TYPE, // device

      HSTRING_TYPE, // argout

      0,

    //-- kTANGO_ATTR_LIST

    "tango_get_dev_attr_list\0",

    F_EXTERNAL,

    NT_FP64,

      HSTRING_TYPE, // device

      HSTRING_TYPE, // argout

      0,

    //-- kTANGO_BLACK_BOX
    "tango_get_dev_black_box\0",
    F_EXTERNAL,
    NT_FP64,
      HSTRING_TYPE, // device
      HSTRING_TYPE, // argout
      NT_FP64,      // n
      0,

    //-- kTANGO_ERROR_STACK
    "tango_get_error_stack\0",
    F_EXTERNAL,
    NT_FP64,
      0,

    //-- kTANGO_ADD_MON_ATTR'
    "tango_start_attr_monitor\0",
    F_EXTERNAL,
    NT_FP64, // error code or client id
      HSTRING_TYPE, // device
      HSTRING_TYPE, // attribute 
      HSTRING_TYPE, // destination
      NT_FP64,      // polling period
      0,

    //-- kTANGO_REM_MON_ATTR
    "tango_stop_attr_monitor\0",
    F_EXTERNAL,
    NT_FP64, // error code
      HSTRING_TYPE, // device
      HSTRING_TYPE, // attribute
      NT_FP64,      // client id
      0,

    //-- kTANGO_SUSPEND_MON
    "tango_suspend_attr_monitor\0",
    F_EXTERNAL,
    NT_FP64, // error code
      HSTRING_TYPE, // device
      HSTRING_TYPE, // attribute
      NT_FP64,      // client id
      0,

    //-- kTANGO_RESUME_MON
    "tango_resume_attr_monitor\0",
    F_EXTERNAL,
    NT_FP64, // error code
      HSTRING_TYPE, // device
      HSTRING_TYPE, // attribute 
      NT_FP64,      // client id
      0,

    //-- kTANGO_CHANGE_MON_PP
    "tango_set_attr_monitor_period\0",
    F_EXTERNAL,
    NT_FP64, // error code
      HSTRING_TYPE, // device
      HSTRING_TYPE, // attribute  
      NT_FP64,      // polling period
      NT_FP64,      // client id
      0,

    //-- kTANGO_COMPUTE_IMG_PROG
    "tango_compute_image_proj\0",
    F_EXTERNAL,
    NT_FP64, // error code
      WAVE_TYPE, // dest_wav
      WAVE_TYPE, // src_wav
      NT_FP64,   // proj_mode
      NT_FP64,   // roi_x0
      NT_FP64,   // roi_y0
      NT_FP64,   // roi_x1
      NT_FP64, // roi_y1
      0,
  0,

END

