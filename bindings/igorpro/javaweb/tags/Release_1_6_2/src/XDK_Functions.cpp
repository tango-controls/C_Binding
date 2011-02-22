// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Functions.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// ABOUT EXTERNAL FUNCTIONS PARAMETERS
//-----------------------------------------------------------------------------
// Parameters are passed to the external function in REVERSE order . The first 
// element in the <parameters structure> is the last parameter of the function. 
// The last element in the <parameters structure> is used to store the returned 
// value (i.e the xfunc result).
// In the XOPF 1100 resource you MUST define the parameters in the "natural" 
// order (the user's order). 
// To avoid numerical problems and for future compatibility, WaveMetrics 
// recommends that you use double-precision (DOUBLE) for all numeric params 
// and results. In other words :
// The structure associated with a function like "res = myfunc(0.35,126,14.2)" 
// is something like : struct {
//                              DOUBLE Param3;  <= 14.2
//                              DOUBLE Param2;  <= 126
//                              DOUBLE Param1;  <= 0.35
//  							              DOUBLE Result;  => result "goes" in res
//                             }; 
//=============================================================================

//=============================================================================
// DIRECT vs MESSAGE CALL METHOD
//-----------------------------------------------------------------------------
// WaveMetrics says :
// The shorcuts that Igor takes in calling an external function by the direct 
// method require that you observe the following restrictions in a direct 
// method function. The XFunc must not do anything that causes the Macintosh 
// LoadSeg trap to run. The XFunc can not do anything that causes, directly 
// or indirectly, a resource to be load.
//=============================================================================

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include "XDK_StandardHeaders.h"

#if defined(_XOP_ADDS_FUNCTIONS_)

#include "XDK_Utils.h"
#include "XDK_Functions.h"
#include "TangoBinding.h"

//=============================================================================
// XFUNCS PARAMETERS 
//=============================================================================
//-----------------------------------------------------------------------------
// STRUCTURES MUST BE 2-byte-aligned
//-----------------------------------------------------------------------------
#if XDK_TARGET_PLATFORM == _XDK_WIN32_
# pragma pack(2)
#elif XDK_TARGET_PLATFORM == _XDK_MACOS_
# pragma options align=mac68k
#endif
//-----------------------------------------------------------------------------
// T_VoidStruct  
//-----------------------------------------------------------------------------
typedef struct {
  DOUBLE result;
} T_VoidStruct;
//-----------------------------------------------------------------------------
// T_VoidStruct  
//-----------------------------------------------------------------------------
typedef struct {
  DOUBLE argin;
  Handle dev;
  DOUBLE result;
} T_HNDLDBLStruct;
//-----------------------------------------------------------------------------
// T_HNDLx1Struct;
//-----------------------------------------------------------------------------
typedef struct {
  Handle dev;
  DOUBLE result;
} T_HNDLx1Struct;
//-----------------------------------------------------------------------------
// T_HNDLx2Struct;
//-----------------------------------------------------------------------------
typedef struct {
  Handle arg;
  Handle dev;
  DOUBLE result;
} T_HNDLx2Struct;
//-----------------------------------------------------------------------------
// T_HNDLx3Struct; 
//-----------------------------------------------------------------------------
typedef struct {
  Handle arg;
  Handle attr;
  Handle dev;
  DOUBLE result;
} T_HNDLx3Struct; 
//-----------------------------------------------------------------------------
// T_HNDLx2DBLx1Struct;
//-----------------------------------------------------------------------------
typedef struct {
  DOUBLE value;
  Handle argout;
  Handle dev;
  DOUBLE result;
} T_HNDLx2DBLx1Struct;
//-----------------------------------------------------------------------------
// T_InOutHNDLStruct;
//-----------------------------------------------------------------------------
typedef struct {
  Handle dev;
  Handle argout;
} T_InOutHNDLStruct;
//-----------------------------------------------------------------------------
// T_TangoCmdInOutStruct  
//-----------------------------------------------------------------------------
typedef struct {
  Handle argout;
  Handle argin;
  Handle cmd;
  Handle dev;
  DOUBLE result;
} T_TangoCmdInOutStruct;
//-----------------------------------------------------------------------------
// RESET STRUCTURES ALIGNMENT
//-----------------------------------------------------------------------------
#if XDK_TARGET_PLATFORM == _XDK_WIN32_
# pragma pack()
#elif XDK_TARGET_PLATFORM == _XDK_MACOS_
# pragma options align=reset
#endif

//=============================================================================
// XDK_Functions member : call_mode                                                  
//=============================================================================
long XFUNC::call_mode (long _xfunc_id)
{
	switch (_xfunc_id) {
    case kTANGO_OPEN_DEV:               
      return (long)XFUNC::tango_open_device;
      break;
    case kTANGO_CLOSE_DEV:               
      return (long)XFUNC::tango_close_device;
      break;
    case kTANGO_CMD_INOUT:               
      return (long)XFUNC::tango_command_inout;
      break;
    case kTANGO_READ_ATTR:               
      return (long)XFUNC::tango_read_attribute;
      break;
    case kTANGO_READ_ATTRS:               
      return (long)XFUNC::tango_read_attributes;
      break;
    case kTANGO_WRITE_ATTR:               
      return (long)XFUNC::tango_write_attribute;
      break;
    case kTANGO_WRITE_ATTRS:               
      return (long)XFUNC::tango_write_attributes;
      break;
    case kTANGO_GET_DEV_STATUS:               
      return (long)XFUNC::tango_get_dev_status;
      break;
    case kTANGO_PING_DEV:               
      return (long)XFUNC::tango_ping_dev;
      break;
    case kTANGO_SET_DEV_TMO:               
      return (long)XFUNC::tango_set_dev_timeout;
      break;
    case kTANGO_GET_DEV_TMO:               
      return (long)XFUNC::tango_get_dev_timeout;
      break;
    case kTANGO_CMD_LIST:               
      return (long)XFUNC::tango_get_dev_cmd_list;
      break;
    case kTANGO_ATTR_LIST:               
      return (long)XFUNC::tango_get_dev_attr_list;
      break;
    case kTANGO_BLACK_BOX:               
      return (long)XFUNC::tango_get_dev_black_box;
      break;
    case kTANGO_ERROR_STACK:               
      return (long)XFUNC::tango_error_stack;
      break;
    default:
      break;
	}
	
  return 0;
}

//=============================================================================
// XDK_Functions member : exec                                               
//=============================================================================
int XFUNC::exec (long _xfunc_id, void *_func_xparams)
{
  switch (_xfunc_id) { 
    case 0:
      //dummy case to make MSVC happy.
    break;
    default:
      XOP_ERROR(("XFUNC::exec::unhandle xfunc call (xfunc id=%d)")); 
      break;
	}

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_open_device
//=============================================================================
int XFUNC::tango_open_device (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  std::string dev; 
  if (XDK_UTILS->handle_to_str(xfp->dev, dev)) {
    XDK_UTILS->set_error("XFUNC::tango_open_device failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_open_device");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->open_device(dev);

  if (xfp->result == kError) {
    std::string d = "failed to open device " + dev;
    XDK_UTILS->push_error("tango_open_device failed",
                          d.c_str(),
                          "XFUNC::tango_open_device");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_close_device
//=============================================================================
int XFUNC::tango_close_device (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  std::string dev; 
  if (XDK_UTILS->handle_to_str(xfp->dev, dev)) {
    XDK_UTILS->set_error("XFUNC::tango_close_device failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_close_device");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->close_device(dev);

  if (xfp->result == kError) {
    std::string d = "failed to close device " + dev;
    XDK_UTILS->push_error("tango_close_device failed",
                          d.c_str(),
                          "XFUNC::tango_close_device");
  }
  
  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_command_inout
//=============================================================================
int XFUNC::tango_command_inout (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_TangoCmdInOutStruct *xfp = static_cast<T_TangoCmdInOutStruct *>(_p);

  std::string dev, cmd, argin, argout; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->cmd, cmd);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->argin, argin);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->argout, argout);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::command_in_out failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::command_in_out");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->command_in_out(dev, cmd, argin, argout);

  if (xfp->result == kError) {
    std::string d = "failed to execute " + cmd;
    d += "(" + argin + ", " + argout + ") on " + dev;
    XDK_UTILS->push_error("tango_command_in_out failed",
                          d.c_str(),
                          "XFUNC::tango_command_in_out");
  }

  return kNoError;
}


//=============================================================================
// XDK_Functions member : tango_read_attribute
//=============================================================================
int XFUNC::tango_read_attribute (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_HNDLx3Struct *xfp = static_cast<T_HNDLx3Struct *>(_p);

  std::string dev, attr, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::tango_read_attribute failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_read_attribute");
    xfp->result = kError;
    return kNoError;
  }

  if (arg.size() == 0) {
    XDK_UTILS->set_error("XFUNC::read_attribute failed",
                         "invalid argout specified (empty)",
                         "XFUNC::read_attribute");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->read_attribute(dev, attr, arg);
  
  if (xfp->result == kError) {
    std::string d = "failed to read attribute " + attr + " on " + dev;
    XDK_UTILS->push_error("tango_read_attribute failed",
                          d.c_str(),
                          "XFUNC::tango_read_attribute");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_read_attributes
//=============================================================================
int XFUNC::tango_read_attributes (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_HNDLx2Struct *xfp = static_cast<T_HNDLx2Struct *>(_p);

  std::string dev, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::tango_read_attributes failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_read_attributes");
    xfp->result = kError;
    return kNoError;
  }

  if (arg.size() == 0) {
    XDK_UTILS->set_error("XFUNC::read_attributes failed",
                         "invalid argout specified (empty)",
                         "XFUNC::read_attributes");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->read_attributes(dev, arg);
  
  if (xfp->result == kError) {
    std::string d = "failed to read " + dev + " attributes";
    XDK_UTILS->push_error("tango_read_attributes failed",
                          d.c_str(),
                          "XFUNC::tango_read_attributes");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_write_attribute
//=============================================================================
int XFUNC::tango_write_attribute (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_HNDLx3Struct *xfp = static_cast<T_HNDLx3Struct *>(_p);

  std::string dev, attr, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->attr, attr);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::write_attribute failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::write_attribute");
    xfp->result = kError;
    return kNoError;
  }

  
  if (arg.size() == 0) {
    XDK_UTILS->set_error("XFUNC::write_attribute failed",
                         "invalid argin specified (empty)",
                         "XFUNC::write_attribute");
    xfp->result = kError;
    return kNoError;
  }


  xfp->result = IDEV_PROXY->write_attribute(dev, attr, arg);
  
  if (xfp->result == kError) {
    std::string d = "failed to write attribute " + attr + " on " + dev;
    XDK_UTILS->push_error("tango_write_attribute failed",
                          d.c_str(),
                          "XFUNC::tango_write_attribute");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_write_attributes
//=============================================================================
int XFUNC::tango_write_attributes (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  int result;
    
  T_HNDLx2Struct *xfp = static_cast<T_HNDLx2Struct *>(_p);

  std::string dev, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("XFUNC::write_attributes failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::write_attributes");
    xfp->result = kError;
    return kNoError;
  }

  if (arg.size() == 0) {
    XDK_UTILS->set_error("XFUNC::write_attributes failed",
                         "invalid argin specified (empty)",
                         "XFUNC::write_attributes");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->write_attributes(dev, arg);

  if (xfp->result == kError) {
    std::string d = "failed to write " + dev + " attributes";
    XDK_UTILS->push_error("tango_writes_attributes failed",
                          d.c_str(),
                          "XFUNC::tango_writes_attributes");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_status
//=============================================================================
int XFUNC::tango_get_dev_status (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_InOutHNDLStruct *xfp = static_cast<T_InOutHNDLStruct *>(_p);

  std::string dev; 
  std::string status;

  int result = XDK_UTILS->handle_to_str(xfp->dev, dev);
  if (result) {
    xfp->argout = 0;
    goto bail;
  }

  result = IDEV_PROXY->status(dev, status);
  if (result) {
    xfp->argout = 0;
    goto bail;
  }

  result = XDK_UTILS->str_to_handle(status, xfp->argout);
  if (result) {
    xfp->argout = 0;
    goto bail;
  }

  return kNoError;

bail:
  std::string d = "failed to get " + dev + " status";
  XDK_UTILS->push_error("tango_get_dev_status failed",
                        d.c_str(),
                        "XFUNC::tango_get_dev_status");

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_ping_dev
//=============================================================================
int XFUNC::tango_ping_dev (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  std::string dev; 
  int result = XDK_UTILS->handle_to_str(xfp->dev, dev);
  if(result) {
    XDK_UTILS->set_error("XFUNC::tango_ping_dev failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_ping_dev");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->ping(dev);

  if (xfp->result == kError) {
    std::string d = dev + " is unreachable";
    XDK_UTILS->push_error("tango_ping_dev failed",
                          d.c_str(),
                          "XFUNC::tango_ping_dev");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_set_dev_timeout
//=============================================================================
int XFUNC::tango_set_dev_timeout (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLDBLStruct *xfp = static_cast<T_HNDLDBLStruct *>(_p);

  int result;
  std::string dev; 
  result = XDK_UTILS->handle_to_str(xfp->dev, dev);
  if(result) {
    XDK_UTILS->set_error("XFUNC::set_timeout failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::set_timeout");
    xfp->result = kError;
    return kNoError;
  }

  int timeout = 0;
  int nt_type = (sizeof(int) == 4) ? NT_I32 : NT_I16;
  ::ConvertData2(&xfp->argin, &timeout, 1, NT_FP64, nt_type);

  xfp->result = IDEV_PROXY->set_timeout(dev, timeout);
  
  if (xfp->result == kError) {
    std::string d = "failed to set " + dev + " timeout";
    XDK_UTILS->push_error("tango_set_dev_timeout failed",
                          d.c_str(),
                          "XFUNC::tango_set_dev_timeout");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_timeout
//=============================================================================
int XFUNC::tango_get_dev_timeout (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx1Struct *xfp = static_cast<T_HNDLx1Struct *>(_p);

  int result;
  std::string dev; 
  result = XDK_UTILS->handle_to_str(xfp->dev, dev);
  if(result) {
    XDK_UTILS->set_error("XFUNC::tango_get_dev_timeout failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_get_dev_timeout");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->get_timeout(dev);
  
  if (xfp->result == kError) {
    std::string d = "failed to get " + dev + " timeout";
    XDK_UTILS->push_error("tango_get_dev_timeout failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_timeout");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_cmd_list
//=============================================================================
int XFUNC::tango_get_dev_cmd_list (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx2Struct *xfp = static_cast<T_HNDLx2Struct *>(_p);

  int result;
  std::string dev, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("tango_get_dev_cmd_list",
                         "internal error (handle_to_str failed)",
                         "XFUNC::tango_get_dev_cmd_list");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->command_list_query(dev, arg);
  
  if (xfp->result == kError) {
    std::string d = "failed to get " + dev + " commands list";
    XDK_UTILS->push_error("tango_get_dev_cmd_list failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_cmd_list");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_attr_list
//=============================================================================
int XFUNC::tango_get_dev_attr_list (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx2Struct *xfp = static_cast<T_HNDLx2Struct *>(_p);

  int result;
  std::string dev, arg; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->arg, arg);
    if (result) break;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("tango_get_dev_attr_list failed",
                         "internal error (handle_to_str failed)",
                         "XFUNC::tango_get_dev_attr_list");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->attribute_list_query(dev, arg);

  if (xfp->result == kError) {
    std::string d = "failed to get " + dev + " attributes list";
    XDK_UTILS->push_error("tango_get_dev_attr_list failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_attr_list");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_get_dev_black_box
//=============================================================================
int XFUNC::tango_get_dev_black_box (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  XDK_UTILS->reset_error();

  T_HNDLx2DBLx1Struct *xfp = static_cast<T_HNDLx2DBLx1Struct *>(_p);

  long n;
  int result;
  std::string dev, argout; 
  do {
    result = XDK_UTILS->handle_to_str(xfp->dev, dev);
    if (result) break;
    result = XDK_UTILS->handle_to_str(xfp->argout, argout);
    if (result) break;
    n = (long)xfp->value;
  } while (0);

  if (result) {
    XDK_UTILS->set_error("tango_get_dev_black_box failed",
                         "XOP internal error (handle_to_str failed)",
                         "XFUNC::tango_get_dev_black_box");
    xfp->result = kError;
    return kNoError;
  }

  xfp->result = IDEV_PROXY->black_box(dev, argout, n);
  
  if (xfp->result == kError) {
    std::string d = "failed to get " + dev + " black box";
    XDK_UTILS->push_error("tango_get_dev_black_box failed",
                          d.c_str(),
                          "XFUNC::tango_get_dev_black_box");
  }

  return kNoError;
}

//=============================================================================
// XDK_Functions member : tango_error_stack
//=============================================================================
int XFUNC::tango_error_stack (void *_p)
{
  if (_p == 0) {
    return kIGOR_INTERNAL_ERROR;
  }

  T_VoidStruct *xfp = static_cast<T_VoidStruct *>(_p);

  xfp->result = XDK_UTILS->export_error();
   
  return kNoError;
}

#endif // _XOP_ADDS_FUNCTIONS_